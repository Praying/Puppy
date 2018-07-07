/*
 * Created by Praying on 2018/1/24.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <base/common/Define.hpp>
#include <base/helper/DebugHelper.hpp>
#include <base/network/MessageBuffer.hpp>

#include <functional>
#include <memory>
#include <queue>
#include <asio/ip/tcp.hpp>
#include "Packet.hpp"
#include <base/common/LockedQueue.hpp>

namespace Flow
{
    namespace Network
    {
        /**
         * Channel used for client or server
         */
        enum class ChannelType
        {
            CHANNEL_TYPE_CLIENT, CHANNEL_TYPE_SERVER
        };

        class Channel;

        typedef std::shared_ptr<Channel> ChannelPtr;

        class Channel : public std::enable_shared_from_this<Channel>
        {
            typedef asio::ip::tcp::socket Socket;
            typedef asio::ip::address Address;
        public:
            /**
             * Constructor
             * @param socket
             */
            explicit Channel(Socket &&socket, ChannelType type);

            /**
             * Destructor
             */
            virtual ~Channel();


            void start();

            //This deal message needed to send in the writeQueue_
            bool updateSend();

            //This deal message has been received in the recvQueue_
            bool updateRecv();

            bool update();

            Address getRemoteIpAddress() const
            {
                return remoteAddress_;
            }

            uint16_t getRemotePort() const
            {
                return remotePort_;
            }

            uint64_t getSocketFd()
            {
                return socket_.native_handle();
            }

            /**
             * Start to connect to a remote address
             * @param ip
             * @param port
             * @param handler
             */
            void asyncConnect(std::string ip, uint16_t port, std::function<void(const asio::error_code &ec)> handler);

            /**
             * @brief Start to read from the socket
             */
            void asyncRead();


            void queueSendPacket(MessageBuffer &&buffer);


            /**
             * Close the socket
             */
            void closeSocket();


            bool isOpen() const
            {
                return !closed_ && !closing_;
            }

            void delayedCloseSocket()
            {
                closing_ = true;
            }

            MessageBuffer &getReadBuffer()
            {
                return readBuffer_;
            }

            /**
             * @brief Send msg by protobuf
             * @param msgID
             * @param msg
             * @param len
             * @return
             */
            bool sendMsgWithoutHeader(const uint16_t msgID, const char *msg, const uint32_t len);

        protected:

            bool asyncProcessQueue();


            void setNoDelay(bool enable)
            {
                asio::error_code err;
                socket_.set_option(asio::ip::tcp::no_delay(enable), err);
                if (err)
                {
                    FLOW_ERROR("Socket::SetNoDelay,failed to set_option(asio::ip::tcp::no_delay) for {0} - {1} ({2})",
                               getRemoteIpAddress().to_string(), err.value(), err.message());
                }
            }


        private:
            void readHandlerInternal(asio::error_code error, size_t transferredByte)
            {
                if (error)
                {
                    closeSocket();
                    return;
                }
                readBuffer_.writeCompleted(transferredByte);
                readHandler();
            }

            /**
             * Read the packet
             */
            void readHandler();

            /**
             * Read the packet header
             * @return
             */
            bool readHeaderHandler();

            /**
             * Read the packet body
             * @return
             */
            bool readDataHandler();

            void writeHandlerWrapper(asio::error_code, std::size_t)
            {
                isWritingAsync_ = false;
                handleSendQueue();
            }

            bool handleSendQueue();

            bool isClient()
            {
                return type_ == ChannelType::CHANNEL_TYPE_CLIENT;
            }

            bool isServer()
            {
                return type_ == ChannelType::CHANNEL_TYPE_SERVER;
            }

        private:
            Socket socket_;
            Address remoteAddress_;
            uint16_t remotePort_;
            MessageBuffer readBuffer_;
            MessageBuffer headerBuffer_;
            MessageBuffer packetBuffer_;
            std::queue<MessageBuffer> writeQueue_;
            LockedQueue<Packet *> recvQueue_;
            std::atomic<bool> closed_;
            std::atomic<bool> closing_;
            bool isWritingAsync_;
            std::size_t sendBufferSize_;
            ChannelType type_;
        };


    }
}
