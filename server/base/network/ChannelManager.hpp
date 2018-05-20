/**
* Created by ABC on 2018/4/15.
*/

#pragma once

#include <folly/Singleton.h>
#include <base/common/Define.hpp>
#include <asio.hpp>

namespace Flow
{
    namespace Network
    {
        class Acceptor;

        class ChannelThread;

        class ChannelManager
        {
            FLOW_SINGLETON_DECLARE(ChannelManager);

        public:
            ChannelManager()
            {
            }

            ~ChannelManager();

            //This start the network listen and ioContext start create thread to run the ioContext
            bool
            startNetwork(asio::io_context &ioContext, const std::string &bindIP, uint16_t clientPort, uint16_t serverPort, int threadCount);

            void stopNetwork();

            void wait();

            void onClientSocketOpen(asio::ip::tcp::socket &&socket, uint32_t threadIndex);

            void onServerSocketOpen(asio::ip::tcp::socket &&socket, uint32_t threadIndex);

            uint32_t getThreadCount() const;

            uint32_t selectThreadWithMinConnections();

            std::pair<asio::ip::tcp::socket *, uint32_t> getSocketForAccept();

            ChannelThread *createThreads();

        private:
            Acceptor *clientAcceptorPtr_ = nullptr;
            Acceptor *serverAcceptorPtr_ = nullptr;
            ChannelThread *threadPtrs_ = nullptr;
            uint32_t threadCount_ = 0;

            int32_t socketSystemSendBufferSize_ = 65535;
            int32_t socketApplicationSendBufferSize_ = -1;
            bool tcpNoDelay_ = true;
        };

#define sChannelManager Network::ChannelManager::instance()
    }
}

