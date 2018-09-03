//
// Created by quran on 2018/9/4.
//

#pragma once

#include <base/network/ImPduBase.hpp>

namespace Flow::DB_PROXY{
    using Network::CImPdu;

    void createGroup(CImPdu* pPdu, uint32_t conn_uuid);

    void getNormalGroupList(CImPdu* pPdu, uint32_t conn_uuid);

    void getGroupInfo(CImPdu* pPdu, uint32_t conn_uuid);

    void modifyMember(CImPdu* pPdu, uint32_t conn_uuid);

    void setGroupPush(CImPdu* pPdu, uint32_t conn_uuid);

    void getGroupPush(CImPdu* pPdu, uint32_t conn_uuid);

}
