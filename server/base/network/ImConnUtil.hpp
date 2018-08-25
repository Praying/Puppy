//
// Created by quran on 2018/8/24.
//

#pragma once

#include <google/protobuf/message_lite.h>
#include <base/network/CImConn.hpp>
namespace Flow::Network {

    using Flow::CImConn;

    int SendMessageLite(CImConn* conn, uint16_t sid, uint16_t cid, const google::protobuf::MessageLite* message);
    int SendMessageLite(CImConn* conn, uint16_t sid, uint16_t cid, uint16_t seq_num, const google::protobuf::MessageLite* message);
    int SendMessageLite(CImConn* conn, uint16_t sid, uint16_t cid, uint16_t seq_num, uint16_t error, const google::protobuf::MessageLite* message);
}

