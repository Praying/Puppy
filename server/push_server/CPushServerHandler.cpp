//
// Created by quran on 2018/8/24.
//

#include <cstdint>
#include "CPushServerHandler.hpp"
#include "PushDefine.hpp"
#include "CPushServer.hpp"
#include "CSessionManager.hpp"

namespace Flow::PushServer{
    void CPushServerHandler::OnClose(uint32_t nsockid)
    {
        PUSH_SERVER_DEBUG("push server closed, sockid: %u.", nsockid);
    }

    void CPushServerHandler::OnAccept(uint32_t nsockid, S_SOCKET sock, const char *szIP,int32_t nPort)
    {
        push_server_ptr pServer = CSessionManager::GetInstance()->GetPushServer();
        push_session_ptr pSession(new CPushSession(pServer->GetIOLoop(), sock));
        CSessionManager::GetInstance()->AddPushSessionBySockID(pSession->GetSocketID(), pSession);
        PUSH_SERVER_INFO("push server accept session, remote ip: %s, port: %u, sockid: %u, real socket: %u.", szIP, nPort, pSession->GetSocketID(), sock);
        pSession->Start();
    }
}