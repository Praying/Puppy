/**
* Created by Praying on 2018/4/5.
*/

#include "ActiveObject.hpp"

namespace Flow
{
    ActiveObject::ActiveObject()
            : done_(false)
    {
        thread_ = std::thread([=]
                              { this->run(); });
    }

    ActiveObject::~ActiveObject()
    {
        send([&]
             { done_ = true; });
        thread_.join();
    }

    void ActiveObject::send(ActiveObject::Message message)
    {
        messageQueue_.push(message);
        condition_.notify_one();
    }

    void ActiveObject::run()
    {
        Message message;
        std::unique_lock<std::mutex> lock(mutex_);
        while (!done_)
        {
            if (condition_.wait_for(lock, std::chrono::milliseconds(1), [this, &message]() -> bool
            {
                return messageQueue_.try_pop(message);
            }))
            {
                message();
            }
        }

    }
}