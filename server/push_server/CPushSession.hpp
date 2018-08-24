//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CPUSHSESSION_HPP
#define SERVER_CPUSHSESSION_HPP


#include <push_server/socket/CTCPSessionAsync.hpp>
#include "CPushSessionHandler.hpp"
#include <memory>

namespace Flow::PushServer{
    class CPushSession : public std::enable_shared_from_this<CPushSession>
    {
    public:
        CPushSession(CEpollIOLoop& io, S_SOCKET sock);
        virtual ~CPushSession();

        uint32_t GetSocketID() { return m_pSession->GetSocketID(); }
        const char* GetRemoteIP() { return m_pSession->GetRemoteIP(); }
        int32_t GetRemotePort() { return m_pSession->GetRemotePort(); }

        BOOL Start();
        BOOL Stop();
        BOOL SendMsg(const char* szMsg, uint32_t nMsgSize);

        void SetHeartBeat(uint64_t nHeartBeat) { m_nLastHeartBeat = nHeartBeat; }
        uint64_t GetLastHeartBeat() { return m_nLastHeartBeat; }
    private:
        uint64_t m_nLastHeartBeat;
        CTCPSessionAsync* m_pSession;
        CEpollIOLoop& m_io;
        CPushSessionHandler m_handler;
    };

    typedef std::shared_ptr<CPushSession> push_session_ptr;
}


#endif //SERVER_CPUSHSESSION_HPP
