//
// Created by quran on 2018/9/3.
//

#pragma once

#include <base/network/ImPduBase.hpp>
namespace Flow::DB_PROXY {

        using Network::CImPdu;
        void doLogin(CImPdu* pPdu, uint32_t conn_uuid);


}

