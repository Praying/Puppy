//
// Created by quran on 2018/8/24.
//

#pragma once

#include <base/network/CImConn.hpp>
#include <base/network/ImPduBase.hpp>
#include "BaseTransferTask.hpp"


namespace Flow::FileServer {


using Network::CImPdu;

// TODO
// 异常情况处理
// 断线、服务器重启等
    class FileClientConn :  public  CImConn {
    public:
        FileClientConn()
                : auth_(false),
                  user_id_(0),
                  transfer_task_(NULL) {
        }

        virtual ~FileClientConn() {}

        virtual void Close();

        // void Close2();

        virtual void OnConnect(net_handle_t handle);

        virtual void OnClose();

        virtual void OnTimer(uint64_t curr_tick);

        virtual void OnWrite();

        virtual void HandlePdu(Network::CImPdu *pdu);

        void ClearTransferTask() {
            user_id_ = 0;
            transfer_task_ = NULL;
        }

    private:
        void _HandleHeartBeat(CImPdu *pdu);

        void _HandleClientFileLoginReq(CImPdu *pdu);

        void _HandleClientFileStates(CImPdu *pdu);

        void _HandleClientFilePullFileReq(CImPdu *pdu);

        void _HandleClientFilePullFileRsp(CImPdu *pdu);

        int _StatesNotify(int state, const std::string &task_id, uint32_t user_id, CImConn *conn);

        // bool _IsAuth() const { return auth_; }

        /// yunfan add 2014.8.18
    private:
        int _PreUpload(const char *task_id);
        //  int _DoUpload(const char* task_id);
        /// yunan add end

        bool auth_;

        uint32_t user_id_;
        // 当前设计每个连接对应一次任务，故可预先缓存
        BaseTransferTask *transfer_task_;
    };

    void InitializeFileClientConn();

    void FileClientConnCallback(void *callback_data, uint8_t msg, uint32_t handle, void *param);

}

