//
// Created by quran on 2018/8/24.
//

#include "ImConnUtil.hpp"
#include "ImPduBase.hpp"
#include "CImConn.hpp"
#include <google/protobuf/message_lite.h>
namespace Flow::Network {

    int SendMessageLite(CImConn* conn, uint16_t sid, uint16_t cid, const ::google::protobuf::MessageLite* message) {
        CImPdu pdu;
        pdu.SetPBMsg(message);
        pdu.SetServiceId(sid);
        pdu.SetCommandId(cid);
        return conn->SendPdu(&pdu);
    }

    int SendMessageLite(CImConn* conn, uint16_t sid, uint16_t cid, uint16_t seq_num, const ::google::protobuf::MessageLite* message) {
        CImPdu pdu;
        pdu.SetPBMsg(message);
        pdu.SetServiceId(sid);
        pdu.SetCommandId(cid);
        pdu.SetSeqNum(seq_num);
        return conn->SendPdu(&pdu);
    }

    int SendMessageLite(CImConn* conn, uint16_t sid, uint16_t cid, uint16_t seq_num, uint16_t error, const ::google::protobuf::MessageLite* message) {
        CImPdu pdu;
        pdu.SetPBMsg(message);
        pdu.SetServiceId(sid);
        pdu.SetCommandId(cid);
        pdu.SetSeqNum(seq_num);
        pdu.SetError(error);
        return conn->SendPdu(&pdu);
    }
}