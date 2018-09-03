//
// Created by quran on 2018/9/4.
//
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <base/common/Util.hpp>
#include <db_proxy_server/CachePool.hpp>
#include <base/proto/ProtosGen/IM.Server.pb.h>
#include <db_proxy_server/CProxyConn.hpp>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <base/proto/ProtosGen/IM.Login.pb.h>
#include <base/proto/ProtosGen/IM.Message.pb.h>
#include "MessageCounter.hpp"
#include "CUserModel.hpp"
#include "CGroupMessageModel.hpp"
#include "MessageModel.hpp"

namespace Flow::DB_PROXY {
    void getUnreadMsgCounter(CImPdu *pPdu, uint32_t conn_uuid) {
        IM::Message::IMUnreadMsgCntReq msg;
        IM::Message::IMUnreadMsgCntRsp msgResp;
        if (msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength())) {
            CImPdu *pPduResp = new CImPdu;

            uint32_t nUserId = msg.user_id();

            std::list<IM::BaseDefine::UnreadInfo> lsUnreadCount;
            uint32_t nTotalCnt = 0;

            CMessageModel::getInstance()->getUnreadMsgCount(nUserId, nTotalCnt, lsUnreadCount);
            CGroupMessageModel::getInstance()->getUnreadMsgCount(nUserId, nTotalCnt, lsUnreadCount);
            msgResp.set_user_id(nUserId);
            msgResp.set_total_cnt(nTotalCnt);
            for (auto it = lsUnreadCount.begin(); it != lsUnreadCount.end(); ++it) {
                IM::BaseDefine::UnreadInfo *pInfo = msgResp.add_unreadinfo_list();
                //            *pInfo = *it;
                pInfo->set_session_id(it->session_id());
                pInfo->set_session_type(it->session_type());
                pInfo->set_unread_cnt(it->unread_cnt());
                pInfo->set_latest_msg_id(it->latest_msg_id());
                pInfo->set_latest_msg_data(it->latest_msg_data());
                pInfo->set_latest_msg_type(it->latest_msg_type());
                pInfo->set_latest_msg_from_user_id(it->latest_msg_from_user_id());
            }


            RAW_LOG(INFO, "userId=%d, unreadCnt=%u, totalCount=%u", nUserId, msgResp.unreadinfo_list_size(), nTotalCnt);
            msgResp.set_attach_data(msg.attach_data());
            pPduResp->SetPBMsg(&msgResp);
            pPduResp->SetSeqNum(pPdu->GetSeqNum());
            pPduResp->SetServiceId(IM::BaseDefine::SID_MSG);
            pPduResp->SetCommandId(IM::BaseDefine::CID_MSG_UNREAD_CNT_RESPONSE);
            CProxyConn::AddResponsePdu(conn_uuid, pPduResp);
        } else {
            RAW_LOG(INFO, "parse pb failed");
        }
    }

    void clearUnreadMsgCounter(CImPdu *pPdu, uint32_t conn_uuid) {
        IM::Message::IMMsgDataReadAck msg;
        if (msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength())) {
            uint32_t nUserId = msg.user_id();
            uint32_t nFromId = msg.session_id();
            IM::BaseDefine::SessionType nSessionType = msg.session_type();
            CUserModel::getInstance()->clearUserCounter(nUserId, nFromId, nSessionType);
            RAW_LOG(INFO, "userId=%u, peerId=%u, type=%u", nFromId, nUserId, nSessionType);
        } else {
            RAW_LOG(INFO, "parse pb failed");
        }
    }

    void setDevicesToken(CImPdu *pPdu, uint32_t conn_uuid) {
        IM::Login::IMDeviceTokenReq msg;
        IM::Login::IMDeviceTokenRsp msgResp;
        if (msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength())) {
            uint32_t nUserId = msg.user_id();
            std::string strToken = msg.device_token();
            CImPdu *pPduResp = new CImPdu;
            CacheManager *pCacheManager = CacheManager::getInstance();
            CacheConn *pCacheConn = pCacheManager->GetCacheConn("token");
            if (pCacheConn) {
                IM::BaseDefine::ClientType nClientType = msg.client_type();
                std::string strValue;
                if (IM::BaseDefine::CLIENT_TYPE_IOS == nClientType) {
                    strValue = "ios:" + strToken;
                } else if (IM::BaseDefine::CLIENT_TYPE_ANDROID == nClientType) {
                    strValue = "android:" + strToken;
                } else {
                    strValue = strToken;
                }

                std::string strOldValue = pCacheConn->get("device_" + int2string(nUserId));

                if (!strOldValue.empty()) {
                    size_t nPos = strOldValue.find(":");
                    if (nPos != std::string::npos) {
                        std::string strOldToken = strOldValue.substr(nPos + 1);
                        std::string strReply = pCacheConn->get("device_" + strOldToken);
                        if (!strReply.empty()) {
                            std::string strNewValue("");
                            pCacheConn->set("device_" + strOldToken, strNewValue);
                        }
                    }
                }

                pCacheConn->set("device_" + int2string(nUserId), strValue);
                std::string strNewValue = int2string(nUserId);
                pCacheConn->set("device_" + strToken, strNewValue);

                RAW_LOG(INFO, "setDeviceToken. userId=%u, deviceToken=%s", nUserId, strToken.c_str());
                pCacheManager->RelCacheConn(pCacheConn);
            } else {
                RAW_LOG(INFO, "no cache connection for token");
            }

            RAW_LOG(INFO, "setDeviceToken. userId=%u, deviceToken=%s", nUserId, strToken.c_str());
            msgResp.set_attach_data(msg.attach_data());
            msgResp.set_user_id(nUserId);
            pPduResp->SetPBMsg(&msgResp);
            pPduResp->SetSeqNum(pPdu->GetSeqNum());
            pPduResp->SetServiceId(IM::BaseDefine::SID_LOGIN);
            pPduResp->SetCommandId(IM::BaseDefine::CID_LOGIN_RES_DEVICETOKEN);
            CProxyConn::AddResponsePdu(conn_uuid, pPduResp);
        } else {
            RAW_LOG(INFO, "parse pb failed");
        }
    }


    void getDevicesToken(CImPdu *pPdu, uint32_t conn_uuid) {
        IM::Server::IMGetDeviceTokenReq msg;
        IM::Server::IMGetDeviceTokenRsp msgResp;
        if (msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength())) {
            CacheManager *pCacheManager = CacheManager::getInstance();
            CacheConn *pCacheConn = pCacheManager->GetCacheConn("token");
            CImPdu *pPduResp = new CImPdu;
            uint32_t nCnt = msg.user_id_size();

            // 对于ios，不推送
            // 对于android，由客户端处理
            bool is_check_shield_status = false;
            time_t now = time(NULL);
            struct tm *_tm = localtime(&now);
            if (_tm->tm_hour >= 22 || _tm->tm_hour <= 7) {
                is_check_shield_status = true;
            }
            if (pCacheConn) {
                std::vector<std::string> vecTokens;
                for (uint32_t i = 0; i < nCnt; ++i) {
                    std::string strKey = "device_" + Util::int2string(msg.user_id(i));
                    vecTokens.push_back(strKey);
                }
                std::map<std::string, std::string> mapTokens;
                bool bRet = pCacheConn->mget(vecTokens, mapTokens);
                pCacheManager->RelCacheConn(pCacheConn);

                if (bRet) {
                    for (auto it = mapTokens.begin(); it != mapTokens.end(); ++it) {
                        std::string strKey = it->first;
                        size_t nPos = strKey.find("device_");
                        if (nPos != std::string::npos) {
                            std::string strUserId = strKey.substr(nPos + strlen("device_"));
                            uint32_t nUserId = Util::string2int(strUserId);
                            std::string strValue = it->second;
                            nPos = strValue.find(":");
                            if (nPos != std::string::npos) {
                                std::string strType = strValue.substr(0, nPos);
                                std::string strToken = strValue.substr(nPos + 1);
                                IM::BaseDefine::ClientType nClientType = IM::BaseDefine::ClientType(0);
                                if (strType == "ios") {
                                    // 过滤出已经设置勿打扰并且为晚上22：00～07：00
                                    uint32_t shield_status = 0;
                                    if (is_check_shield_status) {
                                        CUserModel::getInstance()->getPushShield(nUserId, &shield_status);
                                    }

                                    if (shield_status == 1) {
                                        // 对IOS处理
                                        continue;
                                    } else {
                                        nClientType = IM::BaseDefine::CLIENT_TYPE_IOS;
                                    }

                                    // nClientType = IM::BaseDefine::CLIENT_TYPE_IOS;
                                    // end
                                } else if (strType == "android") {
                                    nClientType = IM::BaseDefine::CLIENT_TYPE_ANDROID;
                                }
                                if (IM::BaseDefine::ClientType_IsValid(nClientType)) {
                                    IM::BaseDefine::UserTokenInfo *pToken = msgResp.add_user_token_info();
                                    pToken->set_user_id(nUserId);
                                    pToken->set_token(strToken);
                                    pToken->set_user_type(nClientType);
                                    uint32_t nTotalCnt = 0;
                                    CMessageModel::getInstance()->getUnReadCntAll(nUserId, nTotalCnt);
                                    CGroupMessageModel::getInstance()->getUnReadCntAll(nUserId, nTotalCnt);
                                    pToken->set_push_count(nTotalCnt);
                                    pToken->set_push_type(1);
                                } else {
                                    RAW_LOG(INFO, "invalid clientType.clientType=%u", nClientType);
                                }
                            } else {
                                RAW_LOG(INFO, "invalid value. value=%s", strValue.c_str());
                            }

                        } else {
                            RAW_LOG(INFO, "invalid key.key=%s", strKey.c_str());
                        }
                    }
                } else {
                    RAW_LOG(INFO, "mget failed!");
                }
            } else {
                RAW_LOG(INFO, "no cache connection for token");
            }

            RAW_LOG(INFO, "req devices token.reqCnt=%u, resCnt=%u", nCnt, msgResp.user_token_info_size());

            msgResp.set_attach_data(msg.attach_data());
            pPduResp->SetPBMsg(&msgResp);
            pPduResp->SetSeqNum(pPdu->GetSeqNum());
            pPduResp->SetServiceId(IM::BaseDefine::SID_OTHER);
            pPduResp->SetCommandId(IM::BaseDefine::CID_OTHER_GET_DEVICE_TOKEN_RSP);
            CProxyConn::AddResponsePdu(conn_uuid, pPduResp);
        } else {
            RAW_LOG(INFO, "parse pb failed");
        }
    }
}