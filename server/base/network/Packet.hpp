/*
 * Created by Praying on 2018/1/26.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <base/common/ByteBuffer.hpp>
#include <boost/endian/conversion.hpp>

namespace Flow
{

#pragma pack(push, 1)

    struct IMsgHead
    {
        enum Head
        {
            HEAD_LENGTH = 8,
        };

        virtual int encode(char *strData) = 0;

        virtual int decode(const char *strData) = 0;

        virtual uint16_t getMsgID() const = 0;

        virtual void setMsgID(uint16_t msgID) = 0;

        virtual uint16_t getMsgType() const = 0;

        virtual void setMsgType(uint16_t type) = 0;

        virtual uint32_t getBodyLength() const = 0;

        virtual void setBodyLength(uint32_t length) = 0;

    };

    struct MsgHead : public IMsgHead
    {
        int encode(char *strData) override
        {
            uint32_t offset = 0;
            uint16_t msgID = boost::endian::native_to_big<uint16_t>(msgID_);
            memcpy(strData + offset, (void *) (&msgID), sizeof(msgID_));
            offset += sizeof(msgID_);

            uint16_t type = boost::endian::native_to_big<uint16_t>(type_);
            memcpy(strData + offset, (void *) (&type), sizeof(type_));
            offset += sizeof(type_);

            uint32_t packetSize = size_ + HEAD_LENGTH;
            uint32_t size = boost::endian::native_to_big<uint32_t>(packetSize);
            memcpy(strData + offset, (void *) (&size), sizeof(size_));
            offset += sizeof(size_);

            if (offset != HEAD_LENGTH)
            {
                assert(0);
            }
            return offset;
        }

        int decode(const char *strData) override
        {
            uint32_t offset = 0;
            uint16_t msgID = 0;
            memcpy(&msgID, strData + offset, sizeof(msgID_));
            msgID_ = boost::endian::big_to_native<uint16_t>(msgID);
            offset += sizeof(msgID_);

            uint16_t type = 0;
            memcpy(&type, strData + offset, sizeof(type_));
            type_ = boost::endian::big_to_native<uint16_t>(type);
            offset += sizeof(type_);

            uint32_t packetSize = 0;
            memcpy(&packetSize, strData + offset, sizeof(size_));
            size_ = boost::endian::big_to_native<uint32_t>(packetSize) - HEAD_LENGTH;
            offset += sizeof(size_);
            if (offset != HEAD_LENGTH)
            {
                assert(0);
            }
            return offset;
        }

        uint16_t getMsgID() const override
        {
            return msgID_;
        }

        void setMsgID(uint16_t msgID) override
        {
            msgID_ = msgID;
        }

        uint32_t getBodyLength() const override
        {
            return size_;
        }

        void setBodyLength(uint32_t length) override
        {
            size_ = length;
        }

        uint16_t getMsgType() const override
        {
            return type_;
        }

        void setMsgType(uint16_t type) override
        {
            type_ = type;
        }

    protected:
        uint32_t size_ = 0;
        uint16_t msgID_ = 0;
        uint16_t type_ = 0;

    };

#pragma pack(pop)

    class Packet : public ByteBuffer
    {
    public:
        Packet()
                : ByteBuffer(0), cmd_(0), param_(0)
        {

        }

        Packet(uint32_t cmd, uint32_t param)
                : cmd_(cmd), param_(param)
        {

        }

        Packet(Packet &&packet)
                : ByteBuffer(std::move(packet)), cmd_(packet.cmd_), param_(packet.param_)
        {

        }

        Packet(const Packet &right)
                : ByteBuffer(right), cmd_(right.cmd_), param_(right.param_)
        {

        }

        Packet(uint32_t cmd, uint32_t param, MessageBuffer &&buffer)
                : ByteBuffer(std::move(buffer)), cmd_(cmd), param_(param)
        {

        }

        Packet &operator=(const Packet &right)
        {
            if (this != &right)
            {
                cmd_ = right.cmd_;
                param_ = right.param_;
                ByteBuffer::operator=(right);
            }
            return *this;
        }

        Packet &operator=(Packet &&right)
        {
            if (this != &right)
            {
                cmd_ = right.cmd_;
                param_ = right.param_;
                ByteBuffer::operator=(std::move(right));
            }
            return *this;
        }

        void initialize(uint32_t cmd, uint32_t param, size_t newSize = 200)
        {
            clear();
            storage_.resize(newSize);
            cmd_ = cmd;
            param_ = param;
        }

        void setCmd(uint32_t cmd)
        {
            cmd_ = cmd;
        }

        uint32_t getCmd() const
        {
            return cmd_;
        }

        void setParam(uint32_t param)
        {
            param_ = param;
        }

        uint32_t getParam() const
        {
            return param_;
        }

        void setCmdAndParam(uint32_t cmd, uint32_t param)
        {
            cmd_ = cmd;
            param_ = param;
        }

    protected:
        uint32_t cmd_;
        uint32_t param_;
    };

}


