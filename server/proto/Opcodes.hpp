/*
 * Created by Praying on 2018/1/26.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <map>
#include <base/network/Packet.hpp>
#include <base/helper/DebugHelper.hpp>
#include <base/network/Channel.hpp>
namespace Flow
{

    typedef std::function<void(Network::ChannelPtr channelPtr, Packet &)> OpcodeHandlerType;
    class OpcodeHandler
    {
    public:
        OpcodeHandler(const char *name, OpcodeHandlerType handler)
                : name_(name),handler_(handler)
        {

        }

        virtual ~OpcodeHandler()
        {
        }

        virtual void call(Network::ChannelPtr channelPtr, Packet &packet) const = 0;

        const char *name_;
        OpcodeHandlerType handler_;
    };


    class ClientOpcodeHandler : public OpcodeHandler
    {
    public:
        ClientOpcodeHandler(const char *name, OpcodeHandlerType handler)
                : OpcodeHandler(name,handler)
        {

        }

        virtual void call(Network::ChannelPtr channelPtr, Packet &packet) const override
        {
        };

    };

    class ServerOpcodeHandler : public OpcodeHandler
    {
    public:
        ServerOpcodeHandler(const char *name, OpcodeHandlerType handler)
                : OpcodeHandler(name, handler)
        {

        }

        virtual void call(Network::ChannelPtr channelPtr, Packet &packet) const override
        {
        };
    };

    class OpcodeTable
    {
    public:
        OpcodeTable();

        OpcodeTable(const OpcodeTable &) = delete;

        OpcodeTable &operator=(const OpcodeTable &) = delete;

        ~OpcodeTable();

        virtual void initialize();
        virtual void initializeClientHandler() = 0;
        virtual void initializeServerHandler() = 0;

        const ClientOpcodeHandler *getClientOpcodeHandler(uint32_t param) const
        {
            try
            {
                return internalTableClient_.at(param);
            }
            catch(std::out_of_range& e)
            {
               FLOW_ERROR("Not found opcode:{}", param);
                return nullptr;
            }

        }

        const ServerOpcodeHandler *getServerOpcodeHandler(uint32_t param) const
        {
            try
            {
                return internalTableServer_.at(param);
            }
            catch(std::out_of_range& e)
            {
                FLOW_ERROR("Not found opcode:{}", param);
                return nullptr;
            }

        }

    protected:
        void validateAndSetServerOpcode(uint32_t opcode, const char *name, OpcodeHandlerType handler);
        void validateAndSetClientOpcode(uint32_t opcode, const char *name, OpcodeHandlerType handler);
    private:
        /*
        template<typename Handler, Handler HandlerFunction>
        void validateAndSetClientOpcode(uint32_t opcode, const char *name);

         */


        /*数组的查找时间为O(1),换成std::map的时间为Log(n)的,但是用数组大小又不确定
         */
        std::map<uint32_t/*opcode*/, ClientOpcodeHandler *> internalTableClient_;
        std::map<uint32_t /*opcode*/, ServerOpcodeHandler *> internalTableServer_;
    };

}


