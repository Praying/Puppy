//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CPROXYTASK_HPP
#define SERVER_CPROXYTASK_HPP

#include <base/network/CTask.hpp>
#include <cstdint>
#include <base/network/ImPduBase.hpp>

namespace Flow {
    typedef void (*pdu_handler_t)(Network::CImPdu* pPdu, uint32_t conn_uuid);
    class CProxyTask : public Flow::Network::CTask {
    public:
        CProxyTask(uint32_t conn_uuid, pdu_handler_t pdu_handler, Network::CImPdu* pPdu);
        virtual ~CProxyTask();

        virtual void run();
    private:
        uint32_t 		m_conn_uuid;
        pdu_handler_t	m_pdu_handler;
        Network::CImPdu* 		m_pPdu;
    };
}


#endif //SERVER_CPROXYTASK_HPP
