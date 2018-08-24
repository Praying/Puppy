//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CSESSIONMANAGER_HPP
#define SERVER_CSESSIONMANAGER_HPP


#include <cstdint>
#include "CPushSession.hpp"
#include "CAPNSClient.hpp"
#include "CPushServer.hpp"
#include <unordered_map>

namespace Flow::PushServer{
    class CSessionManager
    {
    public:
        CSessionManager();
        virtual ~CSessionManager();

        static CSessionManager* GetInstance();
        static void TimerProc(int32_t nIndex, void* param);

        void StartCheckPushSession();
        void StopCheckPushSession();
        void CheckPushSessionTimeOut();
        void CheckPushSessionDelete();

        void AddPushSessionBySockID(uint32_t nsockid, push_session_ptr pSession);
        void RemovePushSessionBySockID(uint32_t nsockid);
        push_session_ptr GetPushSessionBySockID(uint32_t nsockid);
        void ClearPushSession();
        void StopAllPushSession();

        void SetAPNSClient(apns_client_ptr pClient) { m_pAPNSClient = pClient; }
        apns_client_ptr GetAPNSClient() { return m_pAPNSClient; }
        void RemoveAPNSClient();

        void SetPushServer(push_server_ptr pServer) { m_pPushServer = pServer; }
        push_server_ptr GetPushServer() { return m_pPushServer; }
        void RemovePushServer();

    private:
        void _ClearPushSessionForMap();

    private:
        std::unordered_map<uint32_t /* sockid */, push_session_ptr> m_MapPushSessionBySockID;
        CBaseMutex m_MapIOPushSessionBySockIDMutex;

        apns_client_ptr m_pAPNSClient;
        push_server_ptr m_pPushServer;

        CTimer m_checktimer;
    };
}

#endif //SERVER_CSESSIONMANAGER_HPP
