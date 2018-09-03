//
// Created by quran on 2018/9/4.
//
#pragma once

#include <cstdint>
#include <base/network/ImPduBase.hpp>

namespace Flow::DB_PROXY{
    using Network::CImPdu;

    void getUserInfo(CImPdu* pPdu, uint32_t conn_uuid);
    void getChangedUser(CImPdu* pPdu, uint32_t conn_uuid);
    void changeUserSignInfo(CImPdu* pPdu, uint32_t conn_uuid);
    void doPushShield(CImPdu* pPdu, uint32_t conn_uuid);
    void doQueryPushShield(CImPdu* pPdu, uint32_t conn_uuid);
}