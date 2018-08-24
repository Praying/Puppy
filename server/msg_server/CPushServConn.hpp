//
// Created by quran on 2018/8/24.
//

#pragma once

#include <base/network/ImPduBase.hpp>

namespace Flow::MsgServer {
    using namespace Flow::Network;
    
    class CPushServConn : public CImConn
    {
    public:
        CPushServConn();
        virtual ~CPushServConn();

        bool IsOpen() { return m_bOpen; }

        void Connect(const char* server_ip, uint16_t server_port, uint32_t serv_idx);
        virtual void Close();

        virtual void OnConfirm();
        virtual void OnClose();
        virtual void OnTimer(uint64_t curr_tick);

        virtual void HandlePdu(CImPdu* pPdu);
    private:
        void _HandlePushToUserResponse(CImPdu* pPdu);

    private:
        bool 		m_bOpen;
        uint32_t	m_serv_idx;
    };

    void init_push_serv_conn(serv_info_t* server_list, uint32_t server_count);
    CPushServConn* get_push_serv_conn();

    void build_ios_push_flash(std::string& flash, uint32_t msg_type, uint32_t from_id);

}

