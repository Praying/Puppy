/*
 * Created by Praying on 2018/1/28.
 * Copyright (c) Flow. All rights reserved.
 */

#pragma once
#include <deque>
#include <mutex>

namespace Flow
{
    template<typename T, typename StorageType = std::deque<T> >
    class LockedQueue
    {
        std::mutex lock_;
        StorageType queue_;
        volatile bool cannceled_;

    public:
        LockedQueue()
                : cannceled_(false)
        {

        }

        virtual  ~LockedQueue()
        {

        }

        void add(const T &item)
        {
            lock();
            queue_.push_back(item);
            unlock();
        }

        template<typename Iterator>
        void readd(Iterator begin, Iterator end)
        {
            std::lock_guard<std::mutex> lock(lock_);
            queue_.insert(queue_.begin(), begin, end);
        }


        bool next(T &result)
        {
            std::lock_guard<std::mutex> lock(lock_);
            if (queue_.empty())
                return false;
            result = queue_.front();
            queue_.pop_front();
            return true;
        }

        template<typename Checker>
        bool next(T &result, Checker &check)
        {
            std::lock_guard<std::mutex> lock(lock_);
            if (queue_.empty())
                return false;
            result = queue_.front();
            if (!check.process(result))
                return false;
            queue_.pop_front();
            return true;
        }

        T &peek(bool autoUnlock = false)
        {
            lock();
            T &result = queue_.front();
            if (autoUnlock)
                unlock();
            return result;
        }

        void cancel()
        {
            std::lock_guard<std::mutex> lock(lock_);
            cannceled_ = true;
        }

        bool cancelled()
        {
            std::lock_guard<std::mutex> lock(lock_);
            return cannceled_;
        }

        void lock()
        {
            this->lock_.unlock();
        }

        void unlock()
        {
            this->lock_.unlock();
        }

        void pop_front()
        {
            std::lock_guard<std::mutex> lock(lock_);
            queue_.pop_front();
        }

        bool empty()
        {
            std::lock_guard<std::mutex> lock(lock_);
            return queue_.empty();
        }
    };
}
