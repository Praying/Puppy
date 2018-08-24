//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CPUSHSERVERHANDLER_HPP
#define SERVER_CPUSHSERVERHANDLER_HPP

#include <push_server/socket/CBaseHandler.hpp>

namespace Flow::PushServer{
    class CPushServerHandler : public CBaseHandler
    {
    public:
        CPushServerHandler() {}
        virtual ~CPushServerHandler() {}

        void OnAccept(uint32_t nsockid, S_SOCKET sock, const char* szIP, int32_t nPort);
        void OnClose(uint32_t nsockid);
    private:
    };
}



#endif //SERVER_CPUSHSERVERHANDLER_HPP
