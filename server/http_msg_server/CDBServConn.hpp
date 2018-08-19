//
// Created by quran on 2018/8/19.
//

#ifndef SERVER_CDBSERVCONN_HPP
#define SERVER_CDBSERVCONN_HPP

#include <base/network/ImPduBase.hpp>
#include <base/network/CImConn.hpp>
#include <base/common/ServerInfo.hpp>
namespace Flow{
    using Flow::Network::CImPdu;
    using Flow::Network::CImConn;

    class CDBServConn : public CImConn
    {
    public:
        CDBServConn();
        virtual ~CDBServConn();

        bool IsOpen() { return m_bOpen; }

        void Connect(const char* server_ip, uint16_t server_port, uint32_t serv_idx);
        virtual void Close();

        virtual void OnConfirm();
        virtual void OnClose();
        virtual void OnTimer(uint64_t curr_tick);

        virtual void HandlePdu(CImPdu* pPdu);
    private:
        void _HandleStopReceivePacket(CImPdu* pPdu);
        void _HandleCreateGroupRsp(CImPdu* pPdu);
        void _HandleChangeMemberRsp(CImPdu* pPdu);
    private:
        bool 		m_bOpen;
        uint32_t	m_serv_idx;
    };

    void init_db_serv_conn(serv_info_t* server_list, uint32_t server_count, uint32_t concur_conn_cnt);
    CDBServConn* get_db_serv_conn_for_login();
    CDBServConn* get_db_serv_conn();

};


#endif //SERVER_CDBSERVCONN_HPP
