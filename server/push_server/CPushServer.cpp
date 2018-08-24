//
// Created by quran on 2018/8/24.
//

#include <push_server/type/BaseType.hpp>
#include <push_server/socket/CEpollIOLoop.hpp>
#include "CPushServer.hpp"
#include "PushDefine.hpp"

namespace Flow::PushServer{
    CPushServer::CPushServer(CEpollIOLoop& io) : m_io(io)
    {
        m_pServer = new CTCPServer(&m_io);
    }

    CPushServer::~CPushServer()
    {
        if (m_pServer != NULL) {
            delete m_pServer;
            m_pServer = NULL;
        }
    }

    BOOL CPushServer::Start()
    {
        m_pServer->DoAccept.connect(&m_handler, &CPushServerHandler::OnAccept);
        m_pServer->DoClose.connect(&m_handler, &CPushServerHandler::OnClose);

        if (m_pServer->Bind(GetListenIP().c_str(), GetPort()) == FALSE)
        {
            PUSH_SERVER_ERROR("push server bind failed, ip: %s, port: %u.", GetListenIP().c_str(),
                              GetPort());
            return FALSE;
        }

        m_pServer->Listen();
        PUSH_SERVER_DEBUG("push server start successed, ip: %s, port: %u.", GetListenIP().c_str(),
                          GetPort());
        return TRUE;
    }

    BOOL CPushServer::Stop()
    {
        m_pServer->Close();
        return TRUE;
    }
}