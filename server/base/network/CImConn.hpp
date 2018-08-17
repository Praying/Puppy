//
// Created by quran on 2018/8/17.
//

#ifndef SERVER_CIMCONN_HPP
#define SERVER_CIMCONN_HPP

#include <base/common/Util.hpp>
#include <base/common/OSType.hpp>
#include "ImPduBase.hpp"

#define SERVER_HEARTBEAT_INTERVAL    5000
#define SERVER_TIMEOUT                30000
#define CLIENT_HEARTBEAT_INTERVAL    30000
#define CLIENT_TIMEOUT                120000
#define MOBILE_CLIENT_TIMEOUT       60000 * 5
#define READ_BUF_SIZE    2048


namespace Flow::Network {
    using namespace Util;
    class CImConn : public CRefObject {
    public:
        CImConn();

        virtual ~CImConn();

        bool IsBusy() { return m_busy; }

        int SendPdu(CImPdu *pPdu) { return Send(pPdu->GetBuffer(), pPdu->GetLength()); }

        int Send(void *data, int len);

        virtual void OnConnect(net_handle_t handle) { m_handle = handle; }

        virtual void OnConfirm() {}

        virtual void OnRead();

        virtual void OnWrite();

        virtual void OnClose() {}

        virtual void OnTimer(uint64_t curr_tick) {}

        virtual void OnWriteCompelete() {};

        virtual void HandlePdu(CImPdu *pPdu) {}

    protected:
        net_handle_t m_handle;
        bool m_busy;

        std::string m_peer_ip;
        uint16_t m_peer_port;
        CSimpleBuffer m_in_buf;
        CSimpleBuffer m_out_buf;

        bool m_policy_conn;
        uint32_t m_recv_bytes;
        uint64_t m_last_send_tick;
        uint64_t m_last_recv_tick;
        uint64_t m_last_all_user_tick;
    };

    typedef std::unordered_map<net_handle_t, CImConn *> ConnMap_t;
    typedef std::unordered_map<uint32_t, CImConn *> UserMap_t;

    void imconn_callback(void *callback_data, uint8_t msg, uint32_t handle, void *pParam);

    void ReadPolicyFile();

}


#endif //SERVER_CIMCONN_HPP
