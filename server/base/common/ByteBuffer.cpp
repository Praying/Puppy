/*
 * Created by Praying on 2018/1/26.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include <base/common/ByteBuffer.hpp>
#include <base/network/MessageBuffer.hpp>
#include <base/helper/DebugHelper.hpp>
#include <ctime>
#include <cmath>
#include <stdint.h>

namespace Flow
{
    ByteBufferPositionException::ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize)
    {
        std::ostringstream ss;
        ss << "Attempted to " << (add ? "put" : "get") << "value with size:" << valueSize << "in ByteBuffer(pos:" << pos
           << "size:" << size << ")";
        message().assign(ss.str());
    }

    ByteBufferSourceException::ByteBufferSourceException(size_t pos, size_t size, size_t valueSize)
    {
        std::ostringstream ss;
        ss << "Attempted to put a" << (valueSize > 0 ? "Null-pointer" : "zero-sized value") << "in ByteBuffer (pos: "
           << pos << " size:" << size << ")";
        message().assign(ss.str());
    }

    ByteBuffer &ByteBuffer::operator>>(float &value)
    {
        value = read<float>();
        if (!std::isfinite(value))
        {
            throw ByteBufferException();
        }
        return *this;
    }

    ByteBuffer &ByteBuffer::operator>>(double &value)
    {
        value = read<double>();
        if (!std::isfinite(value))
        {
            throw ByteBufferException();
        }
        return *this;
    }

    ByteBuffer::ByteBuffer(MessageBuffer &&buffer)
            : readPos_(0), writePos_(0), storage_(buffer.move())
    {

    }

    uint32_t ByteBuffer::readPackedTime()
    {
        uint32_t packedDate = read<uint32_t>();
        std::tm lt = std::tm();
        lt.tm_min = packedDate & 0x3F;
        lt.tm_hour = (packedDate >> 6) & 0x1F;
        lt.tm_mday = ((packedDate >> 14) & 0x3F) + 1;
        lt.tm_mon = (packedDate >> 20) & 0xF;
        lt.tm_year = ((packedDate >> 24) & 0x1F) + 100;
        return uint32_t(std::mktime(&lt));
    }

    void ByteBuffer::append(uint8_t const *src, size_t count)
    {
        //TODO assert src
        //TODO assert count
        //TODO assert size()<10000000
        size_t const newSize = writePos_ + count;
        if (storage_.capacity() < newSize)
        {
            if (newSize < 100)
            {
                storage_.reserve(300);
            }
            else if (newSize < 750)
            {
                storage_.reserve(2500);
            }
            else if (newSize < 6000)
            {
                storage_.reserve(10000);
            }
            else
            {
                storage_.reserve(400000);
            }

            if (storage_.size() < newSize)
            {
                storage_.resize(newSize);
            }

            std::memcpy(&storage_[writePos_], src, count);
            writePos_ = newSize;

        }


    }

    void ByteBuffer::appendPackedTime(time_t time)
    {
        std::tm lt;
        localtime_r(&time, &lt);
        append<uint32_t>((lt.tm_year - 100) << 24 | lt.tm_mon << 20 | (lt.tm_mday - 1) << 14 | lt.tm_wday << 11 |
                         lt.tm_hour << 6 | lt.tm_min);
    }

    void ByteBuffer::put(size_t pos, const uint8_t *src, size_t count)
    {
        //TODO assert pos + count <= size()
        //TODO assert src
        //TODO assert count
        std::memcpy(&storage_[pos], src, count);
    }

    void ByteBuffer::printStorage() const
    {
        //TODO optimize disabled trace output

        std::ostringstream ss;
        ss << "STORAGE_SIZE: " << size();
        for (uint32_t i = 0; i < size(); ++i)
        {
            ss << read<uint32_t>(i) << " - ";
        }
        ss << " ";
        //FLOW_INFO("[network] {}", ss.str().c_str());
    }

    void ByteBuffer::textLike() const
    {
        std::ostringstream ss;
        ss << "STORAGE_SIZE: " << size();
        for (uint32_t i = 0; i < size(); ++i)
        {
            char buf[2];
            snprintf(buf, 2, "%c", read<uint8_t>(i));
            ss << buf;
        }
        ss << " ";
        //FLOW_INFO("[network] {}", ss.str().c_str());

    }

    void ByteBuffer::hexLike() const
    {
        uint32_t j = 1;
        uint32_t k = 1;
        std::ostringstream ss;
        ss << "STORAGE_SIZE: " << size();
        for (uint32_t i = 0; i < size(); ++i)
        {
            char buf[3];
            snprintf(buf, 3, "%2X ", read<uint8_t>(i));
            if ((i == (j * 8)) && ((i != (k * 16))))
            {
                ss << "|";
                ++j;
            }
            else if (i == (k * 16))
            {
                ss << "\n";
                ++j;
                ++k;
            }
            ss << buf;
        }
        ss << " ";
        //FLOW_INFO("[network], {}", ss.str().c_str());

    }
}