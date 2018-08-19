//
// Created by quran on 2018/8/19.
//

#ifndef SERVER_CROUTESERVCONN_HPP
#define SERVER_CROUTESERVCONN_HPP


#include <cstdint>
#include <base/common/ServerInfo.hpp>
#include <base/network/ImPduBase.hpp>

namespace Flow{
    using namespace Network;
    class CRouteServConn : public CImConn
    {
    public:
        CRouteServConn();
        virtual ~CRouteServConn();

        bool IsOpen() { return m_bOpen; }
        uint64_t GetConnectTime() { return m_connect_time; }

        void Connect(const char* server_ip, uint16_t server_port, uint32_t serv_idx);
        virtual void Close();

        virtual void OnConfirm();
        virtual void OnClose();
        virtual void OnTimer(uint64_t curr_tick);

        virtual void HandlePdu(CImPdu* pPdu);
    private:
        bool 		m_bOpen;
        uint32_t	m_serv_idx;
        uint64_t	m_connect_time;
    };

    void init_route_serv_conn(serv_info_t* server_list, uint32_t server_count);
    bool is_route_server_available();
    void send_to_all_route_server(CImPdu* pPdu);
    CRouteServConn* get_route_serv_conn();

}

#endif //SERVER_CROUTESERVCONN_HPP
