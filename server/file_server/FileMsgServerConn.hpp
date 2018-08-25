//
// Created by quran on 2018/8/25.
//

#pragma once

#include <map>
#include <base/network/CImConn.hpp>
#include <base/network/ImPduBase.hpp>
#include "FileServerUtil.hpp"

namespace Flow::FileServer {
    //using namespace Flow::Network;
    using Network::CImPdu;
    typedef std::map<std::string, transfer_task_t*> TaskMap_t; // on client connect

    class FileMsgServerConn : public CImConn {
    public:
        FileMsgServerConn();
        virtual ~FileMsgServerConn();

        virtual void Close();

        virtual void OnConnect(net_handle_t handle);

        virtual void OnClose();
        virtual void OnTimer(uint64_t curr_tick);

        virtual void OnWrite();
        virtual void HandlePdu(CImPdu* pdu);

    private:
        void _HandleHeartBeat(CImPdu* pdu);
        void _HandleMsgFileTransferReq(CImPdu* pdu);
        void _HandleGetServerAddressReq(CImPdu* pdu);

        bool connected_;
    };


    void InitializeFileMsgServerConn();
    void FileMsgServerConnCallback(void* callback_data, uint8_t msg, uint32_t handle, void* param);


}

