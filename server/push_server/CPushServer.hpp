//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CPUSHSERVER_HPP
#define SERVER_CPUSHSERVER_HPP


#include <push_server/socket/CTCPServer.hpp>
#include "CPushServerHandler.hpp"
#include <memory>
namespace Flow::PushServer{
    class CPushServer : public std::enable_shared_from_this<CPushServer>
    {
    public:
        CPushServer(CEpollIOLoop& io);
        virtual ~CPushServer();

        void SetListenIP(const char* listen_ip) { m_strListenIP = listen_ip; }
        string GetListenIP() { return m_strListenIP; }
        void SetPort(uint32_t port) { m_nPort = port; }
        uint32_t GetPort() { return m_nPort; }
        CEpollIOLoop& GetIOLoop() { return m_io; }

        BOOL Start();
        BOOL Stop();

    private:
        string m_strListenIP;
        uint32_t m_nPort;

        CTCPServer* m_pServer;
        CEpollIOLoop& m_io;
        CPushServerHandler m_handler;
    };

    typedef std::shared_ptr<CPushServer> push_server_ptr;
}


#endif //SERVER_CPUSHSERVER_HPP
