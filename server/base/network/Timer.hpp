/*
 * Created by Praying on 2018/1/25.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <base/helper/DebugHelper.hpp>
#include <chrono>

namespace Flow
{
    inline uint32_t getMSTime()
    {
        using namespace std::chrono;
        static const steady_clock::time_point ApplicationStartTime = steady_clock::now();
        return uint32_t(duration_cast<milliseconds>(steady_clock::now() - ApplicationStartTime).count());
    }

    inline uint32_t getMSTimeDiff(uint32_t oldMSTime, uint32_t newMSTime)
    {
        if (oldMSTime > newMSTime)
            return (0xFFFFFFFF - oldMSTime) + newMSTime;
        else
            return newMSTime - oldMSTime;

    }

    inline uint32_t getMSTimeDiffToNow(uint32_t oldMSTime)
    {
        return getMSTimeDiff(oldMSTime, getMSTime());
    }

    struct IntervalTimer
    {
    public:
        IntervalTimer()
                : interval_(0), current_(0)
        {

        }

        void update(time_t diff)
        {
            current_ += diff;
            if (current_ <= 0)
                current_ = 0;
        }

        bool passed()
        {
            return current_ >= interval_;
        }

        void reset()
        {
            if (current_ >= interval_)
                current_ %= interval_;
        }

        void setCurrent(time_t current)
        {
            current_ = current;
        }

        time_t getCurrent() const
        {
            return current_;
        }

        time_t getInterval() const
        {
            return interval_;
        }

    private:
        time_t interval_;
        time_t current_;
    };


    struct TimeTracker
    {
    public:
        TimeTracker(time_t expiry)
                : expiryTime_(expiry)
        {

        }

        void update(time_t diff)
        {
            expiryTime_ = diff;
        }

        bool passed() const
        {
            return expiryTime_ <= 0;
        }

        void reset(time_t interval)
        {
            expiryTime_ = interval;
        }

        time_t getExpiry() const
        {
            return expiryTime_;
        }

    private:
        time_t expiryTime_;
    };

    struct TimeTrackerSmall
    {
    public:
        TimeTrackerSmall(uint32_t expiry = 0)
                : expiryTime_(expiry)
        {

        }

        void update(uint32_t diff)
        {
            expiryTime_ -= diff;
        }

        bool passed() const
        {
            return expiryTime_ <= 0;
        }

        void reset(uint32_t interval)
        {
            expiryTime_ = interval;
        }

        uint32_t getExpiry() const
        {
            return expiryTime_;
        }


    private:
        uint32_t expiryTime_;
    };

    struct PeriodicTimer
    {
    public:
        PeriodicTimer(uint32_t period, uint32_t startTime)
                : period_(period), expireTime_(startTime)
        {

        }

        bool update(const uint32_t diff)
        {
            if ((expireTime_ -= diff) > 0)
                return false;
            expireTime_ += period_ > uint32_t(diff) ? period_ : diff;
            return true;
        }

        void setPeriodic(uint32_t period, uint32_t startTime)
        {
            expireTime_ = startTime;
            period_ = period;
        }

        //Tracker Interface
        void trackerUpdate(uint32_t diff)
        {
            expireTime_ -= diff;
        }

        bool trackerPassed() const
        {
            return expireTime_ <= 0;
        }

        void trackerReset(uint32_t diff, uint32_t period)
        {
            expireTime_ += period > diff ? period : diff;
        }

    private:
        uint32_t period_;
        uint32_t expireTime_;
    };
}
