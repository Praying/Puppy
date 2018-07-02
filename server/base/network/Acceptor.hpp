/**
* Created by ABC on 2018/4/15.
*/

#pragma once

#include <asio.hpp>
#include <functional>
#include "ChannelManager.hpp"

namespace Flow
{
    namespace Network
    {
        class Acceptor
        {
            typedef void(*AcceptCallback)(asio::ip::tcp::socket &&socket, uint32_t threadIndex);
        public:
            /**
             * Constructor
             * @param ioContext
             * @param bindIp The Ip address to listen
             * @param port The port to listen
             */
            Acceptor(asio::io_context &ioContext, const std::string &bindIp, uint16_t port);

            // This wait for connection from both client and server, the diff is depend on callback
            template<AcceptCallback callback>
            void asyncAcceptWithCallback()
            {
               // asio::ip::tcp::socket *socketPtr;
                //uint32_t threadIndex = 0;
                auto[socketPtr, threadIndex] = ChannelManager::instance()->getSocketForAccept();//newSocketFactory_();
                acceptor_.async_accept(*socketPtr, [this, socketPtr, threadIndex](asio::error_code error)
                {
                    if (!error)
                    {
                        socketPtr->non_blocking(true);
                        callback(std::move(*socketPtr), threadIndex);
                    }
                    if (!closed_)
                    {
                        this->asyncAcceptWithCallback<callback>();
                    }
                });
            }

            bool bind();

            void close();

            void setSocketFactory(std::function<std::pair<asio::ip::tcp::socket *, uint32_t>()> func);

        private:
            std::pair<asio::ip::tcp::socket *, uint32_t> defaultSocketFactory()
            {
                return std::make_pair(&socket_, 0);
            }

            asio::ip::tcp::acceptor acceptor_;
            asio::ip::tcp::endpoint endpoint_;
            asio::ip::tcp::socket socket_;
            std::atomic<bool> closed_;
            std::function<std::pair<asio::ip::tcp::socket *, uint32_t>()> socketFactory_;
        };
    }

}

