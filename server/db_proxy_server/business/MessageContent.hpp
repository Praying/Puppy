//
// Created by quran on 2018/9/4.
//

#pragma once

#include <cstdint>
#include <base/network/ImPduBase.hpp>

namespace Flow::DB_PROXY{
    using Network::CImPdu;

    void getMessage(CImPdu* pPdu, uint32_t conn_uuid);

    void sendMessage(CImPdu* pPdu, uint32_t conn_uuid);

    void getMessageById(CImPdu* pPdu, uint32_t conn_uuid);

    void getLatestMsgId(CImPdu* pPdu, uint32_t conn_uuid);
}