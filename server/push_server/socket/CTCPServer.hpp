//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CTCPSERVER_HPP
#define SERVER_CTCPSERVER_HPP

#include <push_server/sigslot/sigslot.hpp>
#include "SocketIO.hpp"
#include "CIOLoop.hpp"

namespace Flow::PushServer {
    using namespace sigslot;

    class SOCKET_IO_DECLARE_CLASS CTCPServer :
            public CBaseIOStream {
    public:
        CTCPServer(CIOLoop *pIO);

        ~CTCPServer(void);

        void OnAccept();

        void Listen();

        void Close();

        /*uint32_t nsockid, S_SOCKET nSock, const char* szIP, int32_t nPort*/
        sigslot::signal4<uint32_t, S_SOCKET, const char *, int32_t> DoAccept;

        /*uint32_t nsockid*/
        sigslot::signal1 <uint32_t> DoClose;

    private:
    };

}

#endif //SERVER_CTCPSERVER_HPP
