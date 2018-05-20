/*
 * Created by Praying on 2018/2/6.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <condition_variable>
#include <mutex>
#include <queue>
#include <atomic>
#include <type_traits>

namespace Flow
{
    template<typename T>
    class ProducerConsumerQueue
    {
    private:
        std::mutex queueLock_;
        std::queue<T> queue_;
        std::condition_variable condition_;
        std::atomic<bool> shutdown_;

    public:
        ProducerConsumerQueue<T>()
                : shutdown_(false)
        {
        }

        void push(const T &value)
        {
            std::lock_guard<std::mutex> lock(queueLock_);
            queue_.push(std::move(value));
            condition_.notify_one();
        }

        bool empty()
        {
            std::lock_guard<std::mutex> lock(queueLock_);
            return queue_.empty();
        }

        bool pop(T &value)
        {
            std::lock_guard<std::mutex> lock(queueLock_);
            if (queue_.empty() || shutdown_)
                return false;
            value = queue_.front();
            queue_.pop();
            return true;
        }

        void waitAndPop(T &value)
        {
            std::unique_lock<std::mutex> lock(queueLock_);
            while (queue_.empty() && !shutdown_)
                condition_.wait(lock);
            if (queue_.empty() || shutdown_)
                return;
            value = queue_.front();
            queue_.pop();
        }

        void cancel()
        {
            std::unique_lock<std::mutex> lock(queueLock_);
            while (!queue_.empty())
            {
                T &value = queue_.front();
                deleteQueueObject(value);
                queue_.pop();
            }
            shutdown_ = true;
            condition_.notify_all();
        }

    private:
        template<typename E = T>
        typename std::enable_if<std::is_pointer<E>::value>::type deleteQueueObject(E &obj)
        {
            delete obj;
        }

        template<typename E = T>
        typename std::enable_if<std::is_pointer<E>::value>::type deleteQueueObject(const E &)
        {
        };


    };

}