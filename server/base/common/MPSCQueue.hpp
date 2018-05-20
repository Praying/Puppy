/*
 * Created by Praying on 2018/1/28.
 * Copyright (c) Flow. All rights reserved.
 */
#pragma once

#include <atomic>
#include <utility>

namespace Flow
{
    template<typename T>
    class MPSCQueue
    {
    public:
        MPSCQueue()
                : head_(new Node(nullptr)), tail_(head_.load(std::memory_order_relaxed))
        {
            Node *front = head_.load(std::memory_order_relaxed);
            front->next_.store(nullptr, std::memory_order_relaxed);
        }

        ~MPSCQueue()
        {
            T *output;
            while (this->dequeue(output));
            Node *front = head_.load(std::memory_order_relaxed);
            delete front;
        }

        void enqueue(T *input)
        {
            Node *node = new Node(input);
            Node *prevHead = head_.exchange(node, std::memory_order_acq_rel);
            prevHead->next_.store(node, std::memory_order_release);
        }

        bool dequeue(T *&result)
        {
            Node *tail = tail_.load(std::memory_order_relaxed);
            Node *next = tail->next_.load(std::memory_order_acquire);
            if (!next)
                return false;
            result = next->data_;
            tail_.store(next, std::memory_order_release);
            delete tail;
            return true;
        }

    private:
        struct Node
        {
            Node() = delete;

            explicit Node(T *data)
                    : data_(data)
            {
                next_.store(nullptr, std::memory_order::memory_order_relaxed);
            }

            T *data_;
            std::atomic<Node *> next_;
        };

        std::atomic<Node *> head_;
        std::atomic<Node *> tail_;

        MPSCQueue(const MPSCQueue &) = delete;

        MPSCQueue &operator=(const MPSCQueue &)= delete;
    };
}