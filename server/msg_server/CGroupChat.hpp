//
// Created by quran on 2018/8/24.
//

#pragma once

#include <base/network/ImPduBase.hpp>
#include <set>
#include <unordered_map>

namespace Flow::MsgServer {

    using namespace Flow::Network;

    typedef std::set<uint32_t> group_member_t;
    typedef std::unordered_map<uint32_t, group_member_t*> group_map_t;

    class CMsgConn;

    class CGroupChat
    {
    public:
        virtual ~CGroupChat() {}

        static CGroupChat* GetInstance();

        void HandleClientGroupNormalRequest(CImPdu* pPdu, CMsgConn* pFromConn);
        void HandleGroupNormalResponse(CImPdu* pPdu);

        void HandleClientGroupInfoRequest(CImPdu* pPdu, CMsgConn* pFromConn);
        void HandleGroupInfoResponse(CImPdu* pPdu);

        void HandleGroupMessage(CImPdu* pPdu);
        void HandleGroupMessageBroadcast(CImPdu* pPdu);

        void HandleClientGroupCreateRequest(CImPdu* pPdu, CMsgConn* pFromConn);
        void HandleGroupCreateResponse(CImPdu* pPdu);

        void HandleClientGroupChangeMemberRequest(CImPdu* pPdu, CMsgConn* pFromConn);
        void HandleGroupChangeMemberResponse(CImPdu* pPdu);
        void HandleGroupChangeMemberBroadcast(CImPdu* pPdu);

        void HandleClientGroupShieldGroupRequest(CImPdu* pPdu,
                                                 CMsgConn* pFromConn);

        void HandleGroupShieldGroupResponse(CImPdu* pPdu);
        void HandleGroupGetShieldByGroupResponse(CImPdu* pPdu);
    private:
        CGroupChat() {}	// for singleton;

        void _SendPduToUser(CImPdu* pPdu, uint32_t user_id, CMsgConn* pReqConn = NULL);
    private:

        static CGroupChat* s_group_chat_instance;

        group_map_t m_group_map;
    };

}

