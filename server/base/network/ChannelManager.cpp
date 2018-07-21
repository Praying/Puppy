/**
* Created by ABC on 2018/4/15.
*/

#include <base/common/Errors.hpp>
#include <base/helper/DebugHelper.hpp>
#include <base/config/ConfigManager.hpp>
#include <glog/logging.h>
#include "ChannelManager.hpp"
#include "ChannelThread.hpp"
#include "Acceptor.hpp"

namespace Flow
{
    namespace Network
    {
        FLOW_SINGLETON_IMP(ChannelManager)

        namespace
        {
            static void onClientSocketAccept(asio::ip::tcp::socket &&socket, uint32_t threadIndex)
            {
                ChannelManager::instance()->onClientSocketOpen(std::forward<asio::ip::tcp::socket>(socket),
                                                               threadIndex);
            }

            static void onServerSocketAccept(asio::ip::tcp::socket &&socket, uint32_t threadIndex)
            {
                ChannelManager::instance()->onServerSocketOpen(std::forward<asio::ip::tcp::socket>(socket),
                                                               threadIndex);
            }
        }

        ChannelManager::~ChannelManager()
        {
            //FLOW_ASSERT(!threadPtrs_ && !clientAcceptorPtr_ && !serverAcceptorPtr_ && !threadCount_,
            //            "stopNetwork must be called prior to this destructor");
        }

        bool
        ChannelManager::startNetwork(asio::io_context &ioContext, const std::string &bindIP, uint16_t clientPort, uint16_t serverPort, int threadCount)
        {


            tcpNoDelay_ = sConfigManager->getBoolDefault("Network.TcpNodelay", true);
            socketApplicationSendBufferSize_ = sConfigManager->getIntDefault("Network.OutUBuff", 65536);
            socketSystemSendBufferSize_ = sConfigManager->getIntDefault("Network.OutKBuff", -1);
            if (socketApplicationSendBufferSize_ <= 0)
            {
                LOG(ERROR)<< "Network.OutUBuff is wrong in your config file";
                return false;
            }


            //FLOW_ASSERT(threadCount > 0);
            threadCount_ = threadCount;
            threadPtrs_ = createThreads();
            for (auto i = 0; i < threadCount_; ++i)
            {
                threadPtrs_[i].start();
            }

            Acceptor *acceptorPtr = nullptr;
            //Listen on the client port
            try
            {
                acceptorPtr = new Acceptor(ioContext, bindIP, clientPort);

            } catch (asio::system_error &error)
            {

                LOG(ERROR) << "Exception got int startNetwork on address: "<<bindIP<<":"<<clientPort<<"error:"<<error.what();
                return false;
            }

            if (!acceptorPtr->bind())
            {
                LOG(ERROR) <<"Bind client port failed, address: "<<bindIP<<":"<<clientPort;
                return false;
            }

            clientAcceptorPtr_ = acceptorPtr;
            std::function<std::tuple<asio::ip::tcp::socket *, uint32_t>()> func = std::bind(&ChannelManager::getSocketForAccept, this);
            clientAcceptorPtr_->setSocketFactory(func);
            clientAcceptorPtr_->asyncAcceptWithCallback<&onClientSocketAccept>();
            //Listen on the server port
            try
            {
                acceptorPtr = new Acceptor(ioContext, bindIP, serverPort);

            } catch (asio::system_error &error)
            {
                LOG(ERROR) << "Exception got int startNetwork on address: "<<bindIP<<":"<<clientPort<<"error:"<<error.what();
                return false;
            }

            if (!acceptorPtr->bind())
            {
                LOG(ERROR) <<"Bind client port failed, address: "<<bindIP<<":"<<clientPort;
                return false;
            }

            serverAcceptorPtr_ = acceptorPtr;
            //serverAcceptorPtr_->setSocketFactory(func);
            serverAcceptorPtr_->asyncAcceptWithCallback<&onServerSocketAccept>();

            return true;
        }

        void ChannelManager::stopNetwork()
        {
            clientAcceptorPtr_->close();
            serverAcceptorPtr_->close();
            if (threadCount_)
            {
                for (auto i = 0; i < threadCount_; ++i)
                    threadPtrs_[i].stop();
            }

            wait();
            delete clientAcceptorPtr_;
            clientAcceptorPtr_ = nullptr;
            delete serverAcceptorPtr_;
            serverAcceptorPtr_ = nullptr;
            delete[] threadPtrs_;
            threadPtrs_ = nullptr;
            threadCount_ = 0;
        }

        void ChannelManager::wait()
        {
            if (threadCount_)
            {
                for (auto i = 0; i < threadCount_; ++i)
                    threadPtrs_[i].wait();
            }
        }

        void ChannelManager::onClientSocketOpen(asio::ip::tcp::socket &&socket, uint32_t threadIndex)
        {
            if (socketSystemSendBufferSize_ > 0)
            {
                asio::error_code err;
                socket.set_option(asio::socket_base::send_buffer_size(socketSystemSendBufferSize_), err);
                if (err)
                {
                    //FLOW_ERROR(
                    //        "ChannelManager::onClientSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = {}",
                     //       err.message().c_str());
                    return;
                }
            }
            if (tcpNoDelay_)
            {
                asio::error_code err;
                socket.set_option(asio::ip::tcp::no_delay(true), err);
                if (err)
                {
                    //FLOW_ERROR(
                     //       "ChannelManager::onClientSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = {}",
                      //      err.message().c_str());
                    return;
                }
            }

            try
            {
                ChannelPtr newChannel = std::make_shared<Channel>(std::move(socket), ChannelType::CHANNEL_TYPE_CLIENT);
                newChannel->start();
                threadPtrs_[threadIndex].addChannel(newChannel);
            } catch (std::bad_weak_ptr &e)
            {
                //FLOW_ERROR("Bad weak ptr: {}", e.what());
            } catch (asio::error_code &error)
            {
                //FLOW_ERROR("Failed to retrieve client's remote address, error:{}", error.message());
            }


        }

        void ChannelManager::onServerSocketOpen(asio::ip::tcp::socket &&socket, uint32_t threadIndex)
        {
            if (socketSystemSendBufferSize_ > 0)
            {
                asio::error_code err;
                socket.set_option(asio::socket_base::send_buffer_size(socketSystemSendBufferSize_), err);
                if (err)
                {
                    //FLOW_ERROR(
                        //    "ChannelManager::onClientSocketOpen sock.set_option(boost::asio::socket_base::send_buffer_size) err = {}",
                         //   err.message().c_str());
                    return;
                }
            }
            if (tcpNoDelay_)
            {
                asio::error_code err;
                socket.set_option(asio::ip::tcp::no_delay(true), err);
                if (err)
                {
                    //FLOW_ERROR(
                          //  "ChannelManager::onClientSocketOpen sock.set_option(boost::asio::ip::tcp::no_delay) err = {}",
                           // err.message().c_str());
                    return;
                }
            }

            try
            {
                ChannelPtr newChannel = std::make_shared<Channel>(std::move(socket), ChannelType::CHANNEL_TYPE_SERVER);
                newChannel->start();
                threadPtrs_[threadIndex].addChannel(newChannel);
            } catch (std::bad_weak_ptr &e)
            {
                //FLOW_ERROR("Bad weak ptr: {}", e.what());
            } catch (asio::error_code &error)
            {
                //FLOW_ERROR("Failed to retrieve client's remote address, error:{}", error.message());
            }
        }

        uint32_t ChannelManager::getThreadCount() const
        {
            return threadCount_;
        }

        uint32_t ChannelManager::selectThreadWithMinConnections()
        {
            uint32_t minIndex = 0;
            for (uint32_t i = 1; i < threadCount_; ++i)
            {
                if (threadPtrs_[i].getConnectionCount() < threadPtrs_[minIndex].getConnectionCount())
                    minIndex = i;
            }
            return minIndex;
        }

        std::tuple<asio::ip::tcp::socket *, uint32_t> ChannelManager::getSocketForAccept()
        {
            uint32_t threadIndex = selectThreadWithMinConnections();

            return std::make_pair(threadPtrs_[threadIndex].getSocketForAccept(), threadIndex);
        }

        ChannelThread *ChannelManager::createThreads()
        {
            return new ChannelThread[getThreadCount()];
        };
    }
}