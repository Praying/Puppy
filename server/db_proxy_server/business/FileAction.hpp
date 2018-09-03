//
// Created by quran on 2018/9/4.
//
#pragma once

#include <base/network/ImPduBase.hpp>

namespace Flow::DB_PROXY{

    using Network::CImPdu;

    void hasOfflineFile(CImPdu* pPdu, uint32_t conn_uuid);
    void addOfflineFile(CImPdu* pPdu, uint32_t conn_uuid);
    void delOfflineFile(CImPdu* pPdu, uint32_t conn_uuid);
}