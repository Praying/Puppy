/*
 * Created by Praying on 2018/3/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include "OpcodesManager.hpp"

namespace Flow
{
    FLOW_SINGLETON_IMP(OpcodesManager);

    OpcodesManager::OpcodesManager()
    {

    }

    void OpcodesManager::initialize(std::shared_ptr<OpcodeTable> opcodeTable)
    {
        opcodeTable_ =    opcodeTable;
        opcodeTable_->initialize();
    }
    void OpcodesManager::setOpcodeTable(std::shared_ptr<OpcodeTable> opcodeTable)
    {
        opcodeTable_ =    opcodeTable;
        opcodeTable_->initialize();
    }

    const ClientOpcodeHandler *OpcodesManager::getClientOpcodeHandler(uint32_t param) const
    {
        return opcodeTable_?opcodeTable_->getClientOpcodeHandler(param): nullptr;
    }

    const ServerOpcodeHandler *OpcodesManager::getServerOpcodeHandler(uint32_t param) const
    {
        return opcodeTable_?opcodeTable_->getServerOpcodeHandler(param): nullptr;
    }


}