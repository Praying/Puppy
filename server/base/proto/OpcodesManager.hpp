/*
 * Created by Praying on 2018/3/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <base/common/Define.hpp>
#include <base/proto/Opcodes.hpp>

namespace Flow
{

    class OpcodesManager
    {
        FLOW_SINGLETON_DECLARE(OpcodesManager);
    protected:
        OpcodesManager();

    public:
        void initialize(std::shared_ptr<OpcodeTable> opcodeTable);
        void setOpcodeTable(std::shared_ptr<OpcodeTable> opcodesTable);
        const ClientOpcodeHandler *getClientOpcodeHandler(uint32_t param) const;
        const ServerOpcodeHandler *getServerOpcodeHandler(uint32_t param) const;
    private:
        std::shared_ptr<OpcodeTable> opcodeTable_;
    };

}


