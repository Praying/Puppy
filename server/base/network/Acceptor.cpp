/**
* Created by ABC on 2018/4/15.
*/

#include "Acceptor.hpp"
#include "Channel.hpp"

namespace Flow
{
    namespace Network
    {
        Acceptor::Acceptor(asio::io_context &ioContext, const std::string &bindIp, uint16_t port)
                : acceptor_(ioContext), endpoint_(asio::ip::address::from_string(bindIp), port), socket_(ioContext),
                  closed_(false), socketFactory_(std::bind(&Acceptor::defaultSocketFactory, this))
        {
        }


        bool Acceptor::bind()
        {
            asio::error_code errorCode;
            acceptor_.open(endpoint_.protocol(), errorCode);
            if (errorCode)
            {
                FLOW_ERROR("Failed to open acceptor, error:{}", errorCode.message());
                return false;
            }

            acceptor_.bind(endpoint_, errorCode);
            if (errorCode)
            {
                FLOW_ERROR("Failed to bind on {}:{}, error:{}", endpoint_.address().to_string(), endpoint_.port(),
                           errorCode.message());
                return false;
            }

            acceptor_.listen(asio::socket_base::max_listen_connections, errorCode);
            if (errorCode)
            {
                FLOW_ERROR("Failed to listen on {}:{}, error:{}", endpoint_.address().to_string(), endpoint_.port(),
                           errorCode.message());
                return false;
            }

            return true;
        }

        void Acceptor::close()
        {
            if (closed_.exchange(true))
            {
                return;
            }
            asio::error_code errorCode;
            acceptor_.close(errorCode);
            if (errorCode)
            {
                FLOW_ERROR("Failed to close acceptor with error:{}", errorCode.message());
                return;
            }
            return;
        }

        void Acceptor::setSocketFactory(std::function<std::pair<asio::ip::tcp::socket *, uint32_t>()> func)
        {
            socketFactory_ = func;
        }
    }
}