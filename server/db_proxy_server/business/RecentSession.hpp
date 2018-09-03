//
// Created by quran on 2018/9/4.
//

#pragma once

#include <cstdint>
#include <base/network/ImPduBase.hpp>

namespace Flow::DB_PROXY{
    using Network::CImPdu;
    void getRecentSession(CImPdu* pPdu, uint32_t conn_uuid);
    void deleteRecentSession(CImPdu* pPdu, uint32_t conn_uuid);
}