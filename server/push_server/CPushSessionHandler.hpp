//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CPUSHSESSIONHANDLER_HPP
#define SERVER_CPUSHSESSIONHANDLER_HPP


#include <push_server/socket/CBaseHandler.hpp>
#include "CPduMsg.hpp"

namespace Flow::PushServer{
    class CPushSessionHandler : public CBaseHandler
    {
    public:
        CPushSessionHandler() { m_NotificationID = 0; }
        virtual ~CPushSessionHandler() {};

        virtual void OnException(uint32_t nsockid, int32_t nErrorCode);

        virtual void OnClose(uint32_t nsockid);

        virtual void OnRecvData(const char* szBuf, int32_t nBufSize);

    private:
        void _HandlePushMsg(const char* szBuf, int32_t nBufSize);
        void _HandleHeartBeat(const char* szBuf, int32_t nBufSize);
    private:
        uint64_t m_NotificationID;
        CPduMsg m_Msg;

    };
}

#endif //SERVER_CPUSHSESSIONHANDLER_HPP
