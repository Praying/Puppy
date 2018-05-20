/*
 * Created by Praying on 2018/1/26.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <exception>
#include <string>
#include <vector>
#include <sstream>
#include <cstring>

namespace Flow
{
    class MessageBuffer;

    class ByteBufferException : public std::exception
    {
    public:
        ~ByteBufferException()throw()
        {
        }

        char const *what() const throw() override
        {
            return msg_.c_str();
        }

    protected:
        std::string &message()throw()
        {
            return msg_;
        }

    private:
        std::string msg_;
    };

    class ByteBufferPositionException : public ByteBufferException
    {
    public:
        ByteBufferPositionException(bool add, size_t pos, size_t size, size_t valueSize);

        ~ByteBufferPositionException()throw()
        {
        }
    };

    class ByteBufferSourceException : public ByteBufferException
    {
    public:
        ByteBufferSourceException(size_t pos, size_t size, size_t valueSize);

        ~ByteBufferSourceException()throw()
        {
        }
    };

    class ByteBuffer
    {
    public:
        const static size_t DEFAULT_SIZE = 0x1000;

        ByteBuffer()
                : readPos_(0), writePos_(0)
        {
            storage_.reserve(DEFAULT_SIZE);
        }

        ByteBuffer(size_t reserve)
                : readPos_(0), writePos_(0)
        {
            storage_.reserve(reserve);
        }

        ByteBuffer(ByteBuffer &&buf)
                : readPos_(buf.readPos_), writePos_(buf.writePos_), storage_(std::move(buf.storage_))
        {
        }

        ByteBuffer(ByteBuffer const &buf)
                : readPos_(buf.readPos_), writePos_(buf.writePos_), storage_(buf.storage_)
        {
        }

        ByteBuffer(MessageBuffer &&buffer);

        ByteBuffer &operator=(ByteBuffer const &right)
        {
            if (this != &right)
            {
                readPos_ = right.readPos_;
                writePos_ = right.writePos_;
                storage_ = right.storage_;
            }

            return *this;
        }

        virtual ~ByteBuffer()
        {
        }

        void clear()
        {
            writePos_ = readPos_ = 0;
            storage_.clear();
        }

        template<typename T>
        void append(T value)
        {
            static_assert(std::is_fundamental<T>::value, "append(compound)");
            //TODO 大小端转换
            this->append((uint8_t *) &value, sizeof(value));

        }

        template<typename T>
        void put(std::size_t pos, T value)
        {
            static_assert(std::is_fundamental<T>::value, "append(compound");
            //TODO 大小端转换
            put(pos, (uint8_t *) &value, sizeof(value));
        }

        ByteBuffer &operator<<(uint8_t value)
        {
            append<uint8_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint16_t value)
        {
            append<uint16_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint32_t value)
        {
            append<uint32_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(uint64_t value)
        {
            append<uint64_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(int8_t value)
        {
            append<int8_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(int16_t value)
        {
            append<int16_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(int32_t value)
        {
            append<int32_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(int64_t value)
        {
            append<int64_t>(value);
            return *this;
        }

        ByteBuffer &operator<<(float value)
        {
            append<float>(value);
            return *this;
        }

        ByteBuffer &operator<<(double value)
        {
            append<double>(value);
            return *this;
        }

        ByteBuffer &operator<<(const std::string &value)
        {
            if (size_t len = value.length())
            {
                append((uint8_t const *) value.c_str(), len);
            }
            append((uint8_t) 0);
            return *this;
        }

        ByteBuffer &operator<<(const char *str)
        {
            if (size_t len = (str ? strlen(str) : 0))
            {
                append((uint8_t const *) str, len);
            }
            append((uint8_t) 0);
            return *this;
        }

        ByteBuffer &operator>>(bool &value)
        {
            value = read<char>() > 0;
            return *this;
        }

        ByteBuffer &operator>>(uint8_t &value)
        {
            value = read<uint8_t>();
            return *this;
        }

        ByteBuffer &operator>>(uint16_t &value)
        {
            value = read<uint16_t>();
            return *this;
        }

        ByteBuffer &operator>>(uint32_t &value)
        {
            value = read<uint32_t>();
            return *this;
        }

        ByteBuffer &operator>>(uint64_t &value)
        {
            value = read<uint64_t>();
            return *this;
        }

        ByteBuffer &operator>>(int8_t &value)
        {
            value = read<int8_t>();
            return *this;
        }

        ByteBuffer &operator>>(int16_t &value)
        {
            value = read<int16_t>();
            return *this;
        }

        ByteBuffer &operator>>(int32_t &value)
        {
            value = read<int32_t>();
            return *this;
        }

        ByteBuffer &operator>>(int64_t &value)
        {
            value = read<int64_t>();
            return *this;
        }

        ByteBuffer &operator>>(float &value);

        ByteBuffer &operator>>(double &value);

        ByteBuffer &operator>>(std::string &value)
        {
            value.clear();
            while (readPos() < size())
            {
                char c = read<char>();
                if (c == 0)
                    break;
                value += c;
            }
            return *this;
        }

        uint8_t &operator[](size_t const pos)
        {
            if (pos >= size())
            {
                throw ByteBufferPositionException(false, pos, 1, size());
            }
            return storage_[pos];
        }

        uint8_t const &operator[](size_t const pos) const
        {
            if (pos >= size())
            {
                throw ByteBufferPositionException(false, pos, 1, size());
            }
            return storage_[pos];
        }

        size_t readPos() const
        {
            return readPos_;
        }

        size_t writePos() const
        {
            return writePos_;
        }

        size_t readPos(size_t readPos)
        {
            readPos_ = readPos;
            return readPos_;
        }

        size_t writePos(size_t writePos)
        {
            writePos_ = writePos;
            return writePos_;
        }

        void readFinish()
        {
            readPos_ = writePos();
        }

        template<typename T>
        void readSkip()
        {
            readSkip(sizeof(T));
        }

        void readSkip(size_t skip)
        {
            if (readPos_ + skip > size())
            {
                throw ByteBufferPositionException(false, readPos_, skip, size());
            }
            readPos_ += skip;
        }

        template<typename T>
        T read()
        {
            T r = read<T>(readPos_);
            readPos_ += sizeof(T);
            return r;
        }

        template<typename T>
        T read(size_t pos) const
        {
            if (pos + sizeof(T) > size())
            {
                throw ByteBufferPositionException(false, pos, sizeof(T), size());
            }
            T value = *((T const *) &storage_[pos]);
            return value;
        }

        void read(uint8_t *dest, size_t len)
        {
            if (readPos_ + len > size())
            {
                throw ByteBufferPositionException(false, readPos_, len, size());

            }
            std::memcpy(dest, &storage_[readPos_], len);
            readPos_ += len;
        }

        void readPackGUID(uint64_t &guid)
        {
            if (readPos() + 1 > size())
            {
                throw ByteBufferPositionException(false, readPos_, 1, size());
            }

            guid = 0;

            uint8_t guidmark = 0;
            (*this) >> guidmark;
            for (int i = 0; i < 8; ++i)
            {
                if (guidmark & (uint8_t(1) << i))
                {
                    if (readPos() + 1 > size())
                    {
                        throw ByteBufferPositionException(false, readPos_, 1, size());
                    }
                    uint8_t bit;
                    (*this) >> bit;
                    guid |= (uint64_t(bit) << (i * 8));
                }
            }
        }

        uint32_t readPackedTime();

        ByteBuffer &readPackedTime(uint32_t &time)
        {
            time = readPackedTime();
            return *this;
        }

        uint8_t *contents()
        {
            if (storage_.empty())
            {
                throw ByteBufferException();
            }
            return storage_.data();
        }

        uint8_t const *contents() const
        {
            if (storage_.empty())
            {
                throw ByteBufferException();
            }
            return storage_.data();
        }

        size_t size() const
        {
            return storage_.size();
        }

        bool empty() const
        {
            return storage_.empty();
        }

        void resize(size_t newSize)
        {
            storage_.resize(newSize, 0);
            readPos_ = 0;
            writePos_ = size();
        }

        void reserve(size_t ressize)
        {
            if (ressize > size())
            {
                storage_.reserve(ressize);
            }
        }

        void append(const char *src, size_t count)
        {
            return append((const uint8_t *) src, count);
        }

        template<class T>
        void append(const T *src, size_t count)
        {
            return append((const uint8_t *) src, count * sizeof(T));
        }

        void append(uint8_t const *src, size_t count);

        void append(ByteBuffer const &buffer)
        {
            if (buffer.writePos())
            {
                append(buffer.contents(), buffer.writePos());
            }
        }

        void appendPackXYZ(float x, float y, float z)
        {
            uint32_t pack = 0;
            pack |= ((int) (x / 0.25f) & 0x7FF);
            pack |= ((int) (y / 0.25f) & 0x7FF) << 11;
            pack |= ((int) (z / 0.25f) & 0x7FF) << 22;
            *this << pack;
        }

        void appendPackGUID(uint64_t guid)
        {
            uint8_t packGUID[8 + 1];
            packGUID[0] = 0;
            size_t size = 1;
            for (uint8_t i = 0; guid != 0; ++i)
            {
                if (guid & 0xFF)
                {
                    packGUID[0] |= uint8_t(1 << i);
                    packGUID[size] = uint8_t(guid & 0xFF);
                    ++size;
                }
                guid >>= 8;
            }
            append(packGUID, size);
        }

        void appendPackedTime(time_t time);

        void put(size_t pos, const uint8_t *src, size_t count);

        void printStorage() const;

        void textLike() const;

        void hexLike() const;

    protected:
        size_t readPos_;
        size_t writePos_;
        std::vector<uint8_t> storage_;
    };

    template<>
    inline std::string ByteBuffer::read<std::string>()
    {
        std::string tmp;
        *this >> tmp;
        return tmp;
    }

    template<>
    inline void ByteBuffer::readSkip<char *>()
    {
        std::string tmp;
        *this >> tmp;
    }

    template<>
    inline void ByteBuffer::readSkip<char *const>()
    {
        readSkip<char *>();
    }

    template<>
    inline void ByteBuffer::readSkip<std::string>()
    {
        readSkip<char *>();
    }


}
