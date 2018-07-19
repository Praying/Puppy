/*
 * Created by Praying on 2018/1/26.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include <base/proto/Opcodes.hpp>
#include <base/helper/DebugHelper.hpp>
#include <base/proto/ProtosGen/Cmd.pb.h>
#include <base/proto/ProtosGen/ClientCmd.pb.h>
#include <base/network/Channel.hpp>


namespace Flow
{
    class PacketHandler : public ClientOpcodeHandler {
    public:
        PacketHandler(const char *name, OpcodeHandlerType handler)
                : ClientOpcodeHandler(name, handler) {

        }

        void call(Network::ChannelPtr channelPtr, Packet &packet) const override
        {
            Packet nicePacket(std::move(packet));
            handler_(channelPtr, nicePacket);
        }
    };

    OpcodeTable::OpcodeTable()
    {
        internalTableClient_.clear();
        internalTableServer_.clear();
    }

    OpcodeTable::~OpcodeTable()
    {
        for (auto &pair:internalTableClient_)
        {
            delete pair.second;
        }
        for (auto &pair:internalTableServer_)
        {
            delete pair.second;
        }
    }

    void OpcodeTable::validateAndSetClientOpcode(uint32_t opcode, const char *name, OpcodeHandlerType handler)
    {
        if (internalTableClient_.find(opcode) != internalTableClient_.end())
        {
            //FLOW_ERROR("Tried to overried client handler of {} with {} (opcode {})", internalTableClient_[opcode]->name_,
              //         name, opcode);
            return;
        }
        if (opcode == 0)
        {
            //FLOW_ERROR("Opcodes {} does not have a value", name);
            return;
        }

        internalTableClient_.insert(std::make_pair(opcode,new PacketHandler(name, handler)));
    }
    void OpcodeTable::validateAndSetServerOpcode(uint32_t opcode, const char *name, OpcodeHandlerType handler)
    {
        if (internalTableServer_.find(opcode) != internalTableServer_.end())
        {
            //FLOW_ERROR("Tried to overried client handler of {} with {} (opcode {})", internalTableServer_[opcode]->name_,
            //     name, opcode);
            return;
        }
        if (opcode == 0)
        {
            //FLOW_ERROR("Opcodes {} does not have a value", name);
            return;
        }

        internalTableServer_.insert(std::make_pair(opcode, new ServerOpcodeHandler(name,handler)));
    }

    void OpcodeTable::initialize()
    {
        initializeClientHandler();
        initializeServerHandler();
    }

}
