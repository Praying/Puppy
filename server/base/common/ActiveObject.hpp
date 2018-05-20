/**
* Created by Praying on 2018/4/5.
*/

#pragma once

#include <functional>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <tbb/concurrent_queue.h>

namespace Flow
{
    class ActiveObject
    {
        /// Messages are implemented as std::function to allow maximum flexibilify for
        /// how a message can be created with support for functions, functors, class members
        /// and most importantly lambdas.
    public:
        typedef std::function<void()> Message;

    public:
        /// Default constructor kicks off the private thread that listens for incoming messages
        ActiveObject();

        /// Default destructor sends an end message and waits for the private thread to complete
        ~ActiveObject();

        /**
         * @brief Sends a message to be handled by the ActiveObject's private thread.
         * @param message The message to process on the private thread.
         */
        void send(Message message);

    private:
        /// Runs the ActiveObject's message loop until an end message is received.
        void run();

        tbb::concurrent_queue<Message> messageQueue_;
        std::thread thread_;
        std::condition_variable condition_;
        std::mutex mutex_;
        bool done_;
    };
}

