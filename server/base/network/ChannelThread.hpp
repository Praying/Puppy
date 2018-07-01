/**
* Created by ABC on 2018/4/15.
*/

#pragma once

#include <asio/steady_timer.hpp>
#include "Channel.hpp"

namespace Flow
{
    namespace Network
    {
        class ChannelThread
        {
        public:
            ChannelThread();

            ~ChannelThread();

            bool start();

            void run();

            void update();

            void stop();

            void wait();

            uint32_t getConnectionCount() const;

            void addChannel(ChannelPtr channelPtr);

            asio::ip::tcp::socket *getSocketForAccept();

            //Move channel from newChannels_ to channels
            void addNewChannels();

        private:
            typedef std::vector<ChannelPtr> ChannelContainer;
            ChannelContainer channels_;
            ChannelContainer newChannels_;
            std::atomic<uint32_t> connectionNum_ ;
            std::atomic<bool> stopped_;
            std::thread *threadPtr_ = nullptr;
            std::mutex newChannelsMutex_;
            asio::io_context ioContext_;
            asio::ip::tcp::socket acceptSocket_;
            asio::steady_timer updateTimer_;
        };
    }
}

