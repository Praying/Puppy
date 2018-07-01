/**
* Created by ABC on 2018/4/15.
*/

#include "ChannelThread.hpp"
#include <chrono>
namespace Flow
{
    namespace Network
    {
        ChannelThread::ChannelThread()
                : ioContext_(1), acceptSocket_(ioContext_), updateTimer_(ioContext_)
        {
        }

        ChannelThread::~ChannelThread()
        {
            stop();
            if (threadPtr_)
            {
                wait();
                delete threadPtr_;
            }
        }

        bool ChannelThread::start()
        {
            if (threadPtr_)
                return false;
            threadPtr_ = new std::thread(&ChannelThread::run, this);
            return true;
        }

        void ChannelThread::run()
        {
            updateTimer_.expires_from_now(std::chrono::milliseconds(10));
            updateTimer_.async_wait(std::bind(&ChannelThread::update, this));
            ioContext_.run();
            newChannels_.clear();
            channels_.clear();
        }

        void ChannelThread::update()
        {
            if (stopped_)
                return;
            updateTimer_.expires_from_now(std::chrono::milliseconds(10));
            updateTimer_.async_wait(std::bind(&ChannelThread::update, this));
            addNewChannels();
            channels_.erase(std::remove_if(channels_.begin(), channels_.end(), [this](ChannelPtr channelPtr)
            {
                if (!channelPtr->update())
                {
                    if (channelPtr->isOpen())
                        channelPtr->closeSocket();
                    --this->connectionNum_;
                    return true;
                }
                return false;
            }), channels_.end());
        }

        void ChannelThread::stop()
        {
            stopped_ = true;
            ioContext_.stop();
        }

        void ChannelThread::wait()
        {
            assert(threadPtr_);
            threadPtr_->join();
            delete threadPtr_;
            threadPtr_ = nullptr;
        }

        uint32_t ChannelThread::getConnectionCount() const
        {
            return connectionNum_;
        }

        void ChannelThread::addChannel(ChannelPtr channelPtr)
        {
            std::lock_guard<std::mutex> lock(newChannelsMutex_);
            ++connectionNum_;
            newChannels_.push_back(channelPtr);
        }

        asio::ip::tcp::socket *ChannelThread::getSocketForAccept()
        {
            return &acceptSocket_;
        }

        void ChannelThread::addNewChannels()
        {
            std::lock_guard<std::mutex> lock(newChannelsMutex_);
            if (newChannels_.empty())
                return;
            for (auto channelPtr:newChannels_)
            {
                if (!channelPtr->isOpen())
                    --connectionNum_;
                else
                    channels_.push_back(channelPtr);
            }
            newChannels_.clear();
        }
    }
}