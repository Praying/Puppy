//
// Created by quran on 2018/8/24.
//


#include <base/proto/ProtosGen/IM.File.pb.h>
#include <base/common/PublicDefine.hpp>
#include <base/network/Netlib.hpp>
#include "TransferTaskManager.hpp"
#include "FileClientConn.hpp"
#include <base/network/ImConnUtil.hpp>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <glog/logging.h>
#include <glog/raw_logging.h>


namespace Flow::FileServer {
    using namespace IM::BaseDefine;
    using Flow::CImConn;
    static ConnMap_t g_file_client_conn_map; // connection with others, on connect insert...
    void FileClientConnCallback(void *callback_data, uint8_t msg, uint32_t handle, void *param) {
        if (msg == NETLIB_MSG_CONNECT) {
            FileClientConn *conn = new FileClientConn();
            conn->OnConnect(handle);
        } else {
            ////LOG(ERROR)<<"!!!error msg: " <<msg;
        }
    }

    void FileClientConnTimerCallback(void *callback_data, uint8_t msg, uint32_t handle, void *pParam) {
        uint64_t cur_time = get_tick_count();
        for (ConnMap_t::iterator it = g_file_client_conn_map.begin(); it != g_file_client_conn_map.end();) {
            ConnMap_t::iterator it_old = it;
            it++;

            FileClientConn *conn = (FileClientConn *) it_old->second;
            conn->OnTimer(cur_time);
        }
    }


    void FileTaskTimerCallback(void *callback_data, uint8_t msg, uint32_t handle, void *pParam) {
        uint64_t tick = get_tick_count();
        TransferTaskManager::GetInstance()->OnTimer(tick);
    }

    void InitializeFileClientConn() {


        netlib_register_timer(FileClientConnTimerCallback, NULL, 1000);
        netlib_register_timer(FileTaskTimerCallback, NULL, 10000);
    }

    void FileClientConn::Close() {
        LOG(INFO) << "close client, handle " << m_handle;

        if (transfer_task_) {
            if (transfer_task_->GetTransMode() == IM::BaseDefine::FILE_TYPE_ONLINE) {
                transfer_task_->set_state(kTransferTaskStateInvalid);

            } else {
                if (transfer_task_->state() >= kTransferTaskStateUploadEnd) {
                    transfer_task_->set_state(kTransferTaskStateWaitingDownload);
                }
            }
            transfer_task_->SetConnByUserID(user_id_, NULL);

            TransferTaskManager::GetInstance()->DeleteTransferTaskByConnClose(transfer_task_->task_id());


            transfer_task_ = NULL;
        }
        auth_ = false;

        if (m_handle != NETLIB_INVALID_HANDLE) {
            netlib_close(m_handle);
            g_file_client_conn_map.erase(m_handle);
        }


        ReleaseRef();
    }


    void FileClientConn::OnConnect(net_handle_t handle) {
        /// yunfan modify 2014.8.7
        m_handle = handle;

        g_file_client_conn_map.insert(std::make_pair(handle, this));
        netlib_option(handle, NETLIB_OPT_SET_CALLBACK, (void *) imconn_callback);
        netlib_option(handle, NETLIB_OPT_SET_CALLBACK_DATA, (void *) &g_file_client_conn_map);

        uint32_t socket_buf_size = NETLIB_MAX_SOCKET_BUF_SIZE;
        netlib_option(handle, NETLIB_OPT_SET_SEND_BUF_SIZE, &socket_buf_size);
        netlib_option(handle, NETLIB_OPT_SET_RECV_BUF_SIZE, &socket_buf_size);
        /// yunfan modify end
    }

    void FileClientConn::OnClose() {
        LOG(ERROR) << "client onclose: handle=" << m_handle;
        Close();
    }

    void FileClientConn::OnTimer(uint64_t curr_tick) {

        if (transfer_task_ && transfer_task_->GetTransMode() == IM::BaseDefine::FILE_TYPE_ONLINE) {
            if (transfer_task_->state() == kTransferTaskStateInvalid) {
                LOG(ERROR) << "Close another online conn, user_id=" << user_id_;
                Close();
                return;
            }
        }
        // transfer_task_->set_state(kTransferTaskStateInvalid);

        if (curr_tick > m_last_recv_tick + CLIENT_TIMEOUT) {
            LOG(ERROR) << "client timeout, user_id=" << user_id_;
            Close();
        }
    }

    void FileClientConn::OnWrite() {
        CImConn::OnWrite();
    }

    void FileClientConn::HandlePdu(CImPdu *pdu) {
        switch (pdu->GetCommandId()) {
            case CID_OTHER_HEARTBEAT:
                _HandleHeartBeat(pdu);
                break;
            case CID_FILE_LOGIN_REQ:
                _HandleClientFileLoginReq(pdu);
                break;

            case CID_FILE_STATE:
                _HandleClientFileStates(pdu);
                break;
            case CID_FILE_PULL_DATA_REQ:
                _HandleClientFilePullFileReq(pdu);
                break;
            case CID_FILE_PULL_DATA_RSP:
                _HandleClientFilePullFileRsp(pdu);
                break;

            default:
                LOG(ERROR) << "no such cmd id: " << pdu->GetCommandId();
                break;
        }
    }

    void FileClientConn::_HandleHeartBeat(CImPdu *pdu) {
        SendPdu(pdu);
    }

    void FileClientConn::_HandleClientFileLoginReq(CImPdu *pdu) {
        IM::File::IMFileLoginReq login_req;
        CHECK_PB_PARSE_MSG(login_req.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));

        uint32_t user_id = login_req.user_id();
        std::string task_id = login_req.task_id();
        IM::BaseDefine::ClientFileRole mode = login_req.file_role();

        LOG(ERROR) << "Client login, user_id=" << user_id << ", task_id=" << task_id << ", file_role=" << mode;

        BaseTransferTask *transfer_task = NULL;

        bool rv = false;
        do {
            // 查找任务是否存在
            transfer_task = TransferTaskManager::GetInstance()->FindByTaskID(task_id);

            if (transfer_task == NULL) {
                if (mode == CLIENT_OFFLINE_DOWNLOAD) {
                    // 文件不存在，检查是否是离线下载，有可能是文件服务器重启
                    // 尝试从磁盘加载
                    transfer_task = TransferTaskManager::GetInstance()->NewTransferTask(task_id, user_id);
                    // 需要再次判断是否加载成功
                    if (transfer_task == NULL) {
                        LOG(ERROR) << "Find task id failed, user_id=" << user_id << ", taks_id=" << task_id << ", mode="
                                   << mode;
                        break;
                    }
                } else {
                    LOG(ERROR) << "Can't find task_id,  user_id=" << user_id << ", taks_id=" << task_id << ", mode="
                               << mode;
                    break;
                }
            }

            // 状态转换
            rv = transfer_task->ChangePullState(user_id, mode);
            if (!rv) {
                // //LOG(ERROR)<<);
                break;
                //
            }

            // Ok
            auth_ = true;
            transfer_task_ = transfer_task;
            user_id_ = user_id;
            // 设置conn
            transfer_task->SetConnByUserID(user_id, this);
            rv = true;

        } while (0);

        IM::File::IMFileLoginRsp login_rsp;
        login_rsp.set_result_code(rv ? 0 : 1);
        login_rsp.set_task_id(task_id);

        Flow::Network::SendMessageLite(this, SID_FILE, CID_FILE_LOGIN_RES, pdu->GetSeqNum(), &login_rsp);

        if (rv) {
            if (transfer_task->GetTransMode() == IM::BaseDefine::FILE_TYPE_ONLINE) {
                if (transfer_task->state() == kTransferTaskStateWaitingTransfer) {
                    CImConn *conn = transfer_task_->GetToConn();
                    if (conn) {
                        _StatesNotify(CLIENT_FILE_PEER_READY, task_id, transfer_task_->from_user_id(), conn);
                    } else {
                        LOG(ERROR) << "to_conn is close, close me!!!";
                        Close();
                    }
                    // _StatesNotify(CLIENT_FILE_PEER_READY, task_id, user_id, this);
                    // transfer_task->StatesNotify(CLIENT_FILE_PEER_READY, task_id, user_id_);
                }
            } else {
                if (transfer_task->state() == kTransferTaskStateWaitingUpload) {

                    OfflineTransferTask *offline = reinterpret_cast<OfflineTransferTask *>(transfer_task);

                    IM::File::IMFilePullDataReq pull_data_req;
                    pull_data_req.set_task_id(task_id);
                    pull_data_req.set_user_id(user_id);
                    pull_data_req.set_trans_mode(IM::BaseDefine::FILE_TYPE_OFFLINE);
                    pull_data_req.set_offset(0);
                    pull_data_req.set_data_size(offline->GetNextSegmentBlockSize());

                    Network::SendMessageLite(this, SID_FILE, CID_FILE_PULL_DATA_REQ, &pull_data_req);

                    LOG(INFO) << "Pull Data Req";
                }
            }
        } else {
            Close();
        }
    }

    void FileClientConn::_HandleClientFileStates(CImPdu *pdu) {
        if (!auth_ || !transfer_task_) {
            LOG(ERROR) << "Recv a client_file_state, but auth is false";
            return;
        }

        IM::File::IMFileState file_state;
        CHECK_PB_PARSE_MSG(file_state.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));

        std::string task_id = file_state.task_id();
        uint32_t user_id = file_state.user_id();
        uint32_t file_stat = file_state.state();

        LOG(ERROR) << "Recv FileState,  user_id=" << user_id << ", taks_id=" << task_id << ", file_stat=" << file_stat;

        // FilePullFileRsp
        bool rv = false;
        do {
            // 检查user_id
            if (user_id != user_id_) {
                LOG(ERROR) << "Received user_id valid, recv_user_id = " << user_id << ", transfer_task.user_id = "
                           << transfer_task_->from_user_id() << ", user_id_ = " << user_id_;
                break;
            }

            // 检查task_id
            if (transfer_task_->task_id() != task_id) {
                LOG(ERROR) << "Received task_id valid, recv_task_id = " << task_id << ", this_task_id = "
                           << transfer_task_->task_id();
                break;
            }

            switch (file_stat) {
                case CLIENT_FILE_CANCEL:
                case CLIENT_FILE_DONE:
                case CLIENT_FILE_REFUSE: {
                    CImConn *im_conn = transfer_task_->GetOpponentConn(user_id);
                    if (im_conn) {
                        im_conn->SendPdu(pdu);
                        RAW_LOG(ERROR, "Task %s %d by user_id %d notify %d, erased", task_id.c_str(), file_stat,
                                user_id, transfer_task_->GetOpponent(
                                user_id));
                    }
                    // notify other client
                    // CFileConn* pConn = (CFileConn*)t->GetOpponentConn(user_id);
                    // pConn->SendPdu(pPdu);

                    // TransferTaskManager::GetInstance()->DeleteTransferTask(task_id);

                    rv = true;
                    break;
                }

                default:
                    //LOG(ERROR) << "Recv valid file_stat: file_state = " << file_stat << ", user_id=" << user_id
                    //          << ", task_id=" << task_id.c_str();
                    break;
            }

            // rv = true;
        } while (0);

        // if (!rv) {
        Close();
        // }

#if 0
        // 查找任务是否存在
    BaseTransferTask* transfer_task = TransferTaskManager::GetInstance()->FindByTaskID(task_id);
    switch (file_stat) {
        case CLIENT_FILE_CANCEL:
        case CLIENT_FILE_DONE:
        case CLIENT_FILE_REFUSE:
        {
            CImConn* im_conn = transfer_task->GetConnByUserID(user_id);
            if (im_conn) {
                im_conn->SendPdu(pdu);
                //LOG(ERROR)<<"Task %s %d by user_id %d notify %d, erased", task_id.c_str(), file_stat, user_id, transfer_task->GetOpponent(user_id));
            }
            // notify other client
            // CFileConn* pConn = (CFileConn*)t->GetOpponentConn(user_id);
            // pConn->SendPdu(pPdu);

            TransferTaskManager::GetInstance()->DeleteTransferTask(task_id);
            break;
        }

        default:
            break;
    }
#endif
    }

// data handler async
// if uuid not found
// return invalid uuid and close socket
// if offline or mobile task
// check if file size too large, write data and ++size
// if realtime task
// if transfer data

    void FileClientConn::_HandleClientFilePullFileReq(CImPdu *pdu) {
        if (!auth_ || !transfer_task_) {
            //LOG(ERROR) << "Recv a client_file_state, but auth is false";
            return;
        }

        IM::File::IMFilePullDataReq pull_data_req;
        CHECK_PB_PARSE_MSG(pull_data_req.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));

        uint32_t user_id = pull_data_req.user_id();
        std::string task_id = pull_data_req.task_id();
        uint32_t mode = pull_data_req.trans_mode();
        uint32_t offset = pull_data_req.offset();
        uint32_t datasize = pull_data_req.data_size();

        LOG(ERROR) << "Recv FilePullFileReq, user_id=" << user_id << ", task_id=" << task_id << ", file_role=" << mode
                   << ", offset=" << offset << ", datasize=" << datasize;

        // rsp
        IM::File::IMFilePullDataRsp pull_data_rsp;
        pull_data_rsp.set_result_code(1);
        pull_data_rsp.set_task_id(task_id);
        pull_data_rsp.set_user_id(user_id);
        pull_data_rsp.set_offset(offset);
        pull_data_rsp.set_file_data("");

        // BaseTransferTask* transfer_task = NULL;
        int rv = -1;

        do {
            // 检查user_id
            if (user_id != user_id_) {
                LOG(ERROR) << "Received user_id valid, recv_user_id = " << user_id << ", transfer_task.user_id = "
                           << transfer_task_->from_user_id() << ", user_id_ = " << user_id_;
                break;
            }

            // 检查task_id
            if (transfer_task_->task_id() != task_id) {
                LOG(ERROR) << "Received task_id valid, recv_task_id = " << task_id << ", this_task_id = "
                           << transfer_task_->task_id();
                break;
            }

            // 离线传输，需要下载文件
            // 在线传输，从发送者拉数据
            // user_id为transfer_task.to_user_id
            if (!transfer_task_->CheckToUserID(user_id)) {
                LOG(ERROR) << "user_id equal transfer_task.to_user_id, but user_id=" << user_id
                           << ", transfer_task.to_user_id=" << transfer_task_->to_user_id();
                break;
            }

            rv = transfer_task_->DoPullFileRequest(user_id, offset, datasize, pull_data_rsp.mutable_file_data());

            if (rv == -1) {
                break;
            }

            pull_data_rsp.set_result_code(0);

            if (transfer_task_->GetTransMode() == IM::BaseDefine::FILE_TYPE_ONLINE) {
                OnlineTransferTask *online = reinterpret_cast<OnlineTransferTask *>(transfer_task_);
                online->SetSeqNum(pdu->GetSeqNum());
                CImConn *conn = transfer_task_->GetOpponentConn(user_id);
                if (conn) {
                    conn->SendPdu(pdu);
                    Network::SendMessageLite(conn, SID_FILE, CID_FILE_PULL_DATA_RSP, pdu->GetSeqNum(), &pull_data_rsp);
                }
                // SendPdu(&pdu);
            } else {
                Network::SendMessageLite(this, SID_FILE, CID_FILE_PULL_DATA_RSP, pdu->GetSeqNum(), &pull_data_rsp);
                if (rv == 1) {
                    _StatesNotify(CLIENT_FILE_DONE, task_id, transfer_task_->from_user_id(), this);
                }
            }

            // rv = 0;
        } while (0);


        if (rv != 0) {
            Close();
        }

    }

    void FileClientConn::_HandleClientFilePullFileRsp(CImPdu *pdu) {
        if (!auth_ || !transfer_task_) {
            //LOG(ERROR) << "auth is false";
            return;
        }

        // 只有rsp
        IM::File::IMFilePullDataRsp pull_data_rsp;
        CHECK_PB_PARSE_MSG(pull_data_rsp.ParseFromArray(pdu->GetBodyData(), pdu->GetBodyLength()));

        uint32_t user_id = pull_data_rsp.user_id();
        std::string task_id = pull_data_rsp.task_id();
        uint32_t offset = pull_data_rsp.offset();
        uint32_t data_size = static_cast<uint32_t>(pull_data_rsp.file_data().length());
        const char *data = pull_data_rsp.file_data().data();

        // //LOG(ERROR)<<"Recv FilePullFileRsp, user_id=%d, task_id=%s, file_role=%d, offset=%d, datasize=%d", user_id, task_id.c_str(), mode, offset, datasize);
        //LOG(INFO) << "Recv FilePullFileRsp, task_id=" << task_id << ", user_id=" << user_id << ", offset=" << offset
        //        << ", data_size=" << data_size;

        int rv = -1;
        do {
            //
            // 检查user_id
            if (user_id != user_id_) {
                //LOG(ERROR) << "Received user_id valid, recv_user_id = " << user_id << ", transfer_task.user_id = "
                //               << transfer_task_->from_user_id() << ", user_id_ = " << user_id_;
                break;
            }

            // 检查task_id
            if (transfer_task_->task_id() != task_id) {
                //LOG(ERROR) << "Received task_id valid, recv_task_id = " << task_id << ", this_task_id = "
                //             << transfer_task_->task_id();
                // Close();
                break;
            }

            rv = transfer_task_->DoRecvData(user_id, offset, data, data_size);
            if (rv == -1) {
                break;
            }

            if (transfer_task_->GetTransMode() == IM::BaseDefine::FILE_TYPE_ONLINE) {
                // 对于在线，直接转发
                OnlineTransferTask *online = reinterpret_cast<OnlineTransferTask *>(transfer_task_);
                pdu->SetSeqNum(online->GetSeqNum());
                // online->SetSeqNum(pdu->GetSeqNum());

                CImConn *conn = transfer_task_->GetToConn();
                if (conn) {
                    conn->SendPdu(pdu);
                }
            } else {
                // 离线
                // all packages recved
                if (rv == 1) {
                    _StatesNotify(CLIENT_FILE_DONE, task_id, user_id, this);
                    // Close();
                } else {
                    OfflineTransferTask *offline = reinterpret_cast<OfflineTransferTask *>(transfer_task_);

                    IM::File::IMFilePullDataReq pull_data_req;
                    pull_data_req.set_task_id(task_id);
                    pull_data_req.set_user_id(user_id);
                    pull_data_req.set_trans_mode(
                            static_cast<IM::BaseDefine::TransferFileType>(offline->GetTransMode()));
                    pull_data_req.set_offset(offline->GetNextOffset());
                    pull_data_req.set_data_size(offline->GetNextSegmentBlockSize());

                    Network::SendMessageLite(this, SID_FILE, CID_FILE_PULL_DATA_REQ, &pull_data_req);
                    // //LOG(ERROR)<<"size not match");
                }
            }

        } while (0);

        if (rv != 0) {
            // -1，出错关闭
            //  1, 离线上传完成
            Close();
        }
    }

    int FileClientConn::_StatesNotify(int state, const std::string &task_id, uint32_t user_id, CImConn *conn) {
        FileClientConn *file_client_conn = reinterpret_cast<FileClientConn *>(conn);

        IM::File::IMFileState file_msg;
        file_msg.set_state(static_cast<ClientFileState>(state));
        file_msg.set_task_id(task_id);
        file_msg.set_user_id(user_id);

        Network::SendMessageLite(conn, SID_FILE, CID_FILE_STATE, &file_msg);

        //LOG(ERROR) << "notify to user " << user_id << " state " << state << " task " << task_id.c_str();
        return 0;
    }


}