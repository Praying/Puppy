/**
* Created by ABC on 2018/4/14.
*/

#include <proto/OpcodesManager.hpp>
#include <proto/ProtosGen/Cmd.pb.h>
#include <base/common/Util.hpp>
#include "Channel.hpp"
#include "base/common/Errors.hpp"
#include <proto/ProtosGen/ClientCmd.pb.h>

namespace Flow
{
    namespace Network
    {
        Channel::Channel(asio::ip::tcp::socket &&socket, ChannelType type)
                : socket_(std::move(socket)), remoteAddress_(socket_.remote_endpoint().address()),
                  remotePort_(socket_.remote_endpoint().port()), readBuffer_(), closed_(false), closing_(false),
                  isWritingAsync_(false), type_(type)
        {
            readBuffer_.resize(READ_BLOCK_SIZE);
            headerBuffer_.resize(MsgHead::HEAD_LENGTH);
        }

        Channel::~Channel()
        {
            closing_ = true;
            asio::error_code error;
            socket_.close(error);
            if (error)
            {
                FLOW_ERROR("Socket close with error: {}", error.message());
            }

        }

        bool Channel::updateSend()
        {
            if (closed_)
                return false;
#ifndef FLOW_SOCKET_USE_IOCP
            if (isWritingAsync_ || (writeQueue_.empty() && !closing_))
                return true;
            for (; handleSendQueue(););
#endif
            return true;
        }

        void
        Channel::asyncConnect(std::string ip, uint16_t port, std::function<void(const asio::error_code &ec)> handler)
        {
            asio::ip::tcp::endpoint ep(asio::ip::address::from_string(ip), port);
            socket_.async_connect(ep, handler);
        }

        void Channel::asyncRead()
        {
            if (!isOpen())
                return;
            readBuffer_.normalize();
            readBuffer_.ensureFreeSpace();
            socket_.async_read_some(asio::buffer(readBuffer_.getWritePointer(), readBuffer_.getRemainingSpace()),
                                    std::bind(&Channel::readHandlerInternal, this,
                                              std::placeholders::_1, std::placeholders::_2));

        }

        void Channel::queueSendPacket(MessageBuffer &&buffer)
        {
            writeQueue_.push(std::move(buffer));
#ifdef FLOW_SOCKET_USE_IOCP
            asyncProcessQueue();
#endif
        }

        void Channel::closeSocket()
        {
            if (closed_.exchange(true))
                return;
            asio::error_code shutdownError;
            socket_.shutdown(asio::socket_base::shutdown_send, shutdownError);
            if (shutdownError)
            {
                FLOW_DEBUG("Socket::CloseSocket:{},errored when shutting down socket:%i ({})",
                           getRemoteIpAddress().to_string().c_str(), shutdownError.value(),
                           shutdownError.message().c_str());
            }
        }

        bool Channel::asyncProcessQueue()
        {
            if (isWritingAsync_)
                return false;
            isWritingAsync_ = true;

#ifdef FLOW_SOCKET_USE_IOCP
            MessageBuffer& buffer = writeQueue_.front();
                socket_.async_write_some(asio::buffer(buffer.getReadPointer(), buffer.getActiveSize()),
                std::bind(&Socket<T, Stream>::writeHandler,this->shared_from_this(),std::placeholders::_1,std::placeholders::_2));
#else
            socket_.async_write_some(asio::null_buffers(),
                                     std::bind(&Channel::writeHandlerWrapper, this->shared_from_this(),
                                               std::placeholders::_1, std::placeholders::_2));

#endif
            return false;
        }

        void Channel::start()
        {
            asyncRead();
        }

        void Channel::readHandler()
        {
            if (!isOpen())
                return;
            MessageBuffer &packet = getReadBuffer();
            while (packet.getActiveSize() > 0)
            {
                if (headerBuffer_.getRemainingSpace() > 0)
                {
                    std::size_t readHeaderSize = std::min(packet.getActiveSize(), headerBuffer_.getRemainingSpace());
                    headerBuffer_.write(packet.getReadPointer(), readHeaderSize);
                    packet.readCompleted(readHeaderSize);
                    if (headerBuffer_.getRemainingSpace() > 0)
                    {
                        FLOW_ASSERT(packet.getActiveSize() == 0);
                        break;
                    }
                    if (!readHeaderHandler())
                    {
                        closeSocket();
                        return;
                    }
                }

                if (packetBuffer_.getRemainingSpace() > 0)
                {
                    std::size_t readDataSize = std::min(packet.getActiveSize(), packetBuffer_.getRemainingSpace());
                    packetBuffer_.write(packet.getReadPointer(), readDataSize);
                    packet.readCompleted(readDataSize);
                    if (packetBuffer_.getRemainingSpace() > 0)
                    {
                        FLOW_ASSERT(packet.getActiveSize() == 0);
                        break;
                    }
                }

                if (!readDataHandler())
                {
                    closeSocket();
                    return;
                }
                headerBuffer_.reset();

            }
            asyncRead();
        }

        bool Channel::readHeaderHandler()
        {
            FLOW_ASSERT(headerBuffer_.getActiveSize() == MsgHead::HEAD_LENGTH);
            MsgHead head;
            head.decode((const char *) headerBuffer_.getReadPointer());
            packetBuffer_.resize(head.getBodyLength());
            return true;
        }

        bool Channel::readDataHandler()
        {
            MsgHead head;
            head.decode((const char *) headerBuffer_.getReadPointer());

            Packet packet(head.getMsgType(), head.getMsgID(), std::move(packetBuffer_));
            recvQueue_.add(new Packet(std::move(packet)));

            return true;
        }

        bool Channel::handleSendQueue()
        {
            if (writeQueue_.empty())
                return false;
            MessageBuffer &queueMessage = writeQueue_.front();
            std::size_t bytesToSend = queueMessage.getActiveSize();
            asio::error_code error;
            std::size_t bytesSent = socket_.write_some(asio::buffer(queueMessage.getReadPointer(), bytesToSend), error);
            if (error)
            {
                if (error == asio::error::would_block || error == asio::error::try_again)
                {
                    return asyncProcessQueue();
                }
                writeQueue_.pop();
                if (closing_ && writeQueue_.empty())
                    closeSocket();
                return false;
            }
            else if (bytesSent == 0)
            {
                writeQueue_.pop();
                if (closing_ && writeQueue_.empty())
                    closeSocket();
                return false;
            }
            else if (bytesSent < bytesToSend)
            {
                queueMessage.readCompleted(bytesSent);
                return asyncProcessQueue();
            }
            writeQueue_.pop();
            if (closing_ && writeQueue_.empty())
                closeSocket();
            return !writeQueue_.empty();
        }

        bool Channel::updateRecv()
        {
            Packet *packet = nullptr;
            while (recvQueue_.next(packet))
            {
                if (isClient())
                {
                    const ClientOpcodeHandler *handler = OpcodesManager::instance()->getClientOpcodeHandler(
                            packet->getParam());
                    if (handler)
                        handler->call(shared_from_this(), *packet);
                }
                else if (isServer())
                {
                    const ServerOpcodeHandler *handler = OpcodesManager::instance()->getServerOpcodeHandler(
                            packet->getParam());
                    if (handler)
                        handler->call(shared_from_this(), *packet);
                }

            }
            return true;;
        }

        bool Channel::update()
        {
            return updateSend() && updateRecv();
        }

        bool Channel::sendMsgWithoutHeader(const uint16_t msgID, const char *msg, const uint32_t len)
        {
            std::string strOutData;
            MessageBuffer buffer;
            int allLen = Util::encodeMsg(CommandType::COMMAND_TYPE_CLIENT, msgID, msg, len, buffer);
            if (allLen == len + MsgHead::HEAD_LENGTH)
            {
                queueSendPacket(std::move(buffer));
                return true;
            }
            return false;
        }
    }
}
