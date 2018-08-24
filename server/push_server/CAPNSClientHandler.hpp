//
// Created by quran on 2018/8/23.
//

#ifndef SERVER_CAPNSCLIENTHANDLER_HPP
#define SERVER_CAPNSCLIENTHANDLER_HPP

#include <push_server/socket/CBaseHandler.hpp>
#include "APNSMsg.hpp"

namespace Flow::PushServer{

    class CAPNSClientHandler : public CBaseHandler
    {
    public:
        CAPNSClientHandler() {}
        virtual ~CAPNSClientHandler() {}

        virtual void OnException(uint32_t nsockid, int32_t nErrorCode);

        virtual void OnClose(uint32_t nsockid);

        virtual void OnConnect(uint32_t nsockid);

        virtual void OnSSLConnect(uint32_t nsockid);

        virtual void OnRecvData(const char* szBuf, int32_t nBufSize);

    protected:
        CAPNSGateWayResMsg m_Msg;
    };
}


#endif //SERVER_CAPNSCLIENT_HPP
