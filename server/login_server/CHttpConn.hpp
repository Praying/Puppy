//
// Created by quran on 2018/8/24.
//

#pragma once

#include <cstdint>
#include <base/common/Util.hpp>
#include <base/common/OSType.hpp>
#include <base/network/ImPduBase.hpp>
#include <base/network/CHttpParserWrapper.hpp>
#include <unordered_map>

#define HTTP_CONN_TIMEOUT			60000

#define READ_BUF_SIZE	2048
#define HTTP_RESPONSE_HTML          "HTTP/1.1 200 OK\r\n"\
                                    "Connection:close\r\n"\
                                    "Content-Length:%d\r\n"\
                                    "Content-Type:text/html;charset=utf-8\r\n\r\n%s"
#define HTTP_RESPONSE_HTML_MAX      1024


namespace Flow::LoginServer {

    using namespace Flow::Network;


    enum {
        CONN_STATE_IDLE,
        CONN_STATE_CONNECTED,
        CONN_STATE_OPEN,
        CONN_STATE_CLOSED,
    };

class CHttpConn : public Util::CRefObject
    {
    public:
        CHttpConn();
        virtual ~CHttpConn();

        uint32_t GetConnHandle() { return m_conn_handle; }
        char* GetPeerIP() { return (char*)m_peer_ip.c_str(); }

        int Send(void* data, int len);

        void Close();
        void OnConnect(net_handle_t handle);
        void OnRead();
        void OnWrite();
        void OnClose();
        void OnTimer(uint64_t curr_tick);
        void OnWriteComlete();
    private:
        void _HandleMsgServRequest(std::string& url, std::string& post_data);

    protected:
        net_handle_t	m_sock_handle;
        uint32_t		m_conn_handle;
        bool			m_busy;

        uint32_t        m_state;
        std::string		m_peer_ip;
        uint16_t		m_peer_port;
        CSimpleBuffer	m_in_buf;
        CSimpleBuffer	m_out_buf;

        uint64_t		m_last_send_tick;
        uint64_t		m_last_recv_tick;

        CHttpParserWrapper m_cHttpParser;
    };

    typedef std::unordered_map<uint32_t, CHttpConn*> HttpConnMap_t;

    CHttpConn* FindHttpConnByHandle(uint32_t handle);
    void init_http_conn();

}

