/*
 * Created by Praying on 2018/1/24.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <vector>
#include <cstring>

namespace Flow
{
    class MessageBuffer
    {
        typedef std::vector<uint8_t>::size_type size_type;

    public:
        MessageBuffer()
                : writePos_(0), readPos_(0), storage_()
        {
            storage_.resize(4096);
        }

        explicit MessageBuffer(std::size_t initiaSize)
                : writePos_(0), readPos_(0), storage_()
        {

        }

        MessageBuffer(const MessageBuffer &right)
                : writePos_(right.writePos_), readPos_(right.readPos_), storage_(right.storage_)
        {

        }

        MessageBuffer(MessageBuffer &&right)
                : writePos_(right.writePos_), readPos_(right.readPos_), storage_(std::move(right.storage_))
        {

        }

        void reset()
        {
            writePos_ = 0;
            readPos_ = 0;
        }

        void resize(size_type bytes)
        {
            storage_.resize(bytes);
        }

        uint8_t *getBasePointer()
        {
            return storage_.data();
        }

        uint8_t *getReadPointer()
        {
            return getBasePointer() + readPos_;
        }

        uint8_t *getWritePointer()
        {
            return getBasePointer() + writePos_;
        }

        void readCompleted(size_type bytes)
        {
            readPos_ += bytes;
        }

        void writeCompleted(size_type bytes)
        {
            writePos_ += bytes;
        }

        size_type getActiveSize() const
        {
            return writePos_ - readPos_;
        }

        size_type getRemainingSpace() const
        {
            return storage_.size() - writePos_;
        }

        size_type getBufferSize() const
        {
            return storage_.size();
        }

        //discards inactive data
        void normalize()
        {
            if (readPos_)
            {
                if (readPos_ != writePos_)
                {
                    memmove(getBasePointer(), getReadPointer(), getActiveSize());
                }
                writePos_ -= readPos_;
                readPos_ = 0;
            }
        }

        void ensureFreeSpace()
        {
            if (getRemainingSpace() == 0)
            {
                storage_.resize(storage_.size() * 3 / 2);
            }
        }

        void write(void const* data, std::size_t size)
        {
            if(size)
            {
                memcpy(getWritePointer(), data, size);
                writeCompleted(size);
            }
        }

        std::vector<uint8_t> &&move()
        {
            writePos_ = 0;
            readPos_ = 0;
            return std::move(storage_);
        }

        MessageBuffer &operator=(const MessageBuffer &right)
        {
            if (this != &right)
            {
                writePos_ = right.writePos_;
                readPos_ = right.readPos_;
                storage_ = right.storage_;
            }
            return *this;
        }

        MessageBuffer &operator=(MessageBuffer &&right)
        {
            if (this != &right)
            {
                writePos_ = right.writePos_;
                readPos_ = right.readPos_;
                storage_ = right.move();
            }
            return *this;
        }

    private:
        size_type writePos_ = 0;
        size_type readPos_ = 0;
        std::vector<uint8_t> storage_;
    };
}
