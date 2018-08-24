//
// Created by quran on 2018/8/24.
//

#pragma once

#include <base/network/ImPduBase.hpp>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <list>
namespace Flow::MsgServer {

    using namespace Flow::Network;

    class CFileServConn : public CImConn
    {
    public:
        CFileServConn();
        virtual ~CFileServConn();

        bool IsOpen() { return m_bOpen; }

        void Connect(const char* server_ip, uint16_t server_port, uint32_t serv_idx);
        virtual void Close();

        virtual void OnConfirm();
        virtual void OnClose();
        virtual void OnTimer(uint64_t curr_tick);

        virtual void HandlePdu(CImPdu* pPdu);

        const std::list<IM::BaseDefine::IpAddr>* GetFileServerIPList() { return &m_ip_list; }

    private:
        void _HandleFileMsgTransRsp(CImPdu* pPdu);
        void _HandleFileServerIPRsp(CImPdu* pPdu);

    private:
        bool 		m_bOpen;
        uint32_t	m_serv_idx;
        uint64_t	m_connect_time;
        std::list<IM::BaseDefine::IpAddr> m_ip_list;
    };

    void init_file_serv_conn(serv_info_t* server_list, uint32_t server_count);
    bool is_file_server_available();
    CFileServConn* get_random_file_serv_conn();
}

