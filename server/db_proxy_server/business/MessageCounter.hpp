//
// Created by quran on 2018/9/4.
//

#pragma once

#include <base/network/ImPduBase.hpp>

namespace Flow::DB_PROXY{
    using Network::CImPdu;

    void getUnreadMsgCounter(CImPdu* pPdu, uint32_t conn_uuid);
    void clearUnreadMsgCounter(CImPdu* pPdu, uint32_t conn_uuid);

    void setDevicesToken(CImPdu* pPdu, uint32_t conn_uuid);
    void getDevicesToken(CImPdu* pPdu, uint32_t conn_uuid);
}