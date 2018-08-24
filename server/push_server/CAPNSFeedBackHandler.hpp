//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CAPNSFEEDBACKHANDLER_HPP
#define SERVER_CAPNSFEEDBACKHANDLER_HPP

#include <push_server/socket/CBaseHandler.hpp>
#include "APNSMsg.hpp"

namespace Flow::PushServer{
    class CAPNSFeedBackHandler : public CBaseHandler
    {
    public:
        CAPNSFeedBackHandler() {}
        virtual ~CAPNSFeedBackHandler() {}

        virtual void OnException(uint32_t nsockid, int32_t nErrorCode);

        virtual void OnClose(uint32_t nsockid);

        virtual void OnConnect(uint32_t nsockid);

        virtual void OnSSLConnect(uint32_t nsockid);

        virtual void OnRecvData(const char* szBuf, int32_t nBufSize);

    private:
        CAPNSFeedBackResMsg m_Msg;
    };
}

#endif //SERVER_CAPNSFEEDBACKHANDLER_HPP
