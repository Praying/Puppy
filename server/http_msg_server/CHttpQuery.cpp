//
// Created by quran on 2018/8/19.
//

#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <base/common/PublicDefine.hpp>
#include <base/network/Netlib.hpp>
#include <base/network/CImConn.hpp>
#include <base/network/ImPduBase.hpp>
#include <glog/logging.h>
#include <list>
#include <base/proto/ProtosGen/IM.Group.pb.h>
#include "CHttpQuery.hpp"
#include "CDbAttachData.hpp"
#include "CPostDataParser.hpp"
#include "CDBServConn.hpp"

namespace Flow {
    using Flow::Network::CImPdu;

    static uint32_t g_total_query = 0;
    static uint32_t g_last_year = 0;
    static uint32_t g_last_month = 0;
    static uint32_t g_last_mday = 0;

    CHttpQuery *CHttpQuery::m_query_instance = NULL;

    std::unordered_map<std::string, auth_struct *> g_hm_http_auth;

    void http_query_timer_callback(void *callback_data, uint8_t msg, uint32_t handle, void *pParam) {
        struct tm *tm;
        time_t currTime;

        time(&currTime);
        tm = localtime(&currTime);

        uint32_t year = tm->tm_year + 1900;
        uint32_t mon = tm->tm_mon + 1;
        uint32_t mday = tm->tm_mday;
        if (year != g_last_year || mon != g_last_month || mday != g_last_mday) {
            // a new day begin, clear the count
            LOG(ERROR) << "a new day begin, g_total_query= " << g_total_query;
            g_total_query = 0;
            g_last_year = year;
            g_last_month = mon;
            g_last_mday = mday;
        }
    }

    CHttpQuery *CHttpQuery::GetInstance() {
        if (!m_query_instance) {
            m_query_instance = new CHttpQuery();
            netlib_register_timer(http_query_timer_callback, NULL, 1000);
        }

        return m_query_instance;
    }

    void CHttpQuery::DispatchQuery(std::string &url, std::string &post_data, CHttpConn *pHttpConn) {
        ++g_total_query;

        /* 在用
         /query/SendMessageByXiaoT
         /query/GroupP2PMessage
         
         */
        LOG(ERROR) << "DispatchQuery, url="<<url<<", content="<<post_data;

        Json::Reader reader;
        Json::Value value;
        Json::Value root;

        if (!reader.parse(post_data, value)) {
            LOG(ERROR) << "json parse failed, post_data= "<< post_data;
            pHttpConn->Close();
            return;
        }

        std::string strErrorMsg;
        std::string strAppKey;
        HTTP_ERROR_CODE nRet = HTTP_ERROR_SUCCESS;
        try {
            std::string strInterface(url.c_str() + strlen("/query/"));
            strAppKey = value["app_key"].asString();
            std::string strIp = pHttpConn->GetPeerIP();
            uint32_t nUserId = value["req_user_id"].asUInt();
            nRet = _CheckAuth(strAppKey, nUserId, strInterface, strIp);
        }
        catch (std::runtime_error msg) {
            nRet = HTTP_ERROR_INTERFACE;
        }

        if (HTTP_ERROR_SUCCESS != nRet) {
            if (nRet < HTTP_ERROR_MAX) {
                root["error_code"] = nRet;
                root["error_msg"] = HTTP_ERROR_MSG[nRet];
            } else {
                root["error_code"] = -1;
                root["error_msg"] = "未知错误";
            }
            std::string strResponse = root.toStyledString();
            pHttpConn->Send((void *) strResponse.c_str(), strResponse.length());
            return;
        }

        // process post request with post content
        if (strcmp(url.c_str(), "/query/CreateGroup") == 0) {
            _QueryCreateGroup(strAppKey, value, pHttpConn);
        } else if (strcmp(url.c_str(), "/query/ChangeMembers") == 0) {
            _QueryChangeMember(strAppKey, value, pHttpConn);
        } else {
            LOG(ERROR) << "url not support ";
            pHttpConn->Close();
            return;
        }
    }

    void CHttpQuery::_QueryCreateGroup(const std::string &strAppKey, Json::Value &post_json_obj, CHttpConn *pHttpConn) {
        CDBServConn *pConn = get_db_serv_conn();
        if (!pConn) {
            LOG(ERROR) << "no connection to DBProxy ";
            char *response_buf = PackSendResult(HTTP_ERROR_SERVER_EXCEPTION, HTTP_ERROR_MSG[9].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["req_user_id"].isNull()) {
            LOG(ERROR) << "no user id ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;

        }

        if (post_json_obj["group_name"].isNull()) {
            LOG(ERROR) << "no group name ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["group_type"].isNull()) {
            LOG(ERROR) << "no group type ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["group_avatar"].isNull()) {
            LOG(ERROR) << "no group avatar ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["user_id_list"].isNull()) {
            LOG(ERROR) << "no user list ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        try {
            uint32_t user_id = post_json_obj["req_user_id"].asUInt();
            std::string group_name = post_json_obj["group_name"].asString();
            uint32_t group_type = post_json_obj["group_type"].asUInt();
            std::string group_avatar = post_json_obj["group_avatar"].asString();
            uint32_t user_cnt = post_json_obj["user_id_list"].size();
            LOG(ERROR) << "QueryCreateGroup, user_id: " << user_id << ", group_name: " << group_name << ", group_type: "
                       << group_type << ", user_cnt: " << user_cnt;

            if (!IM::BaseDefine::GroupType_IsValid(group_type)) {
                LOG(ERROR) << "QueryCreateGroup, unvalid group_type";
                char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
                pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
                pHttpConn->Close();
                return;
            }

            CDbAttachData attach_data(ATTACH_TYPE_HANDLE, pHttpConn->GetConnHandle());
            IM::Group::IMGroupCreateReq msg;
            msg.set_user_id(0);
            msg.set_group_name(group_name);
            msg.set_group_avatar(group_avatar);
            msg.set_group_type((::IM::BaseDefine::GroupType) group_type);
            for (uint32_t i = 0; i < user_cnt; i++) {
                uint32_t member_id = post_json_obj["user_id_list"][i].asUInt();
                msg.add_member_id_list(member_id);
            }
            msg.set_attach_data(attach_data.GetBuffer(), attach_data.GetLength());

            CImPdu pdu;
            pdu.SetPBMsg(&msg);
            pdu.SetServiceId(IM::BaseDefine::SID_GROUP);
            pdu.SetCommandId(IM::BaseDefine::CID_GROUP_CREATE_REQUEST);
            pConn->SendPdu(&pdu);

        }
        catch (std::runtime_error msg) {
            LOG(ERROR) << "parse json data failed.";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
        }

    }

    void
    CHttpQuery::_QueryChangeMember(const std::string &strAppKey, Json::Value &post_json_obj, CHttpConn *pHttpConn) {
        CDBServConn *pConn = get_db_serv_conn();
        if (!pConn) {
            LOG(ERROR) << "no connection to RouteServConn ";
            char *response_buf = PackSendResult(HTTP_ERROR_SERVER_EXCEPTION, HTTP_ERROR_MSG[9].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }
        if (post_json_obj["req_user_id"].isNull()) {
            LOG(ERROR) << "no user id ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["group_id"].isNull()) {
            LOG(ERROR) << "no group id ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["modify_type"].isNull()) {
            LOG(ERROR) << "no modify_type ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        if (post_json_obj["user_id_list"].isNull()) {
            LOG(ERROR) << "no user list ";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
            return;
        }

        try {
            uint32_t user_id = post_json_obj["req_user_id"].asUInt();
            uint32_t group_id = post_json_obj["group_id"].asUInt();
            uint32_t modify_type = post_json_obj["modify_type"].asUInt();
            uint32_t user_cnt = post_json_obj["user_id_list"].size();
            LOG(ERROR) << "QueryChangeMember, user_id: " << user_id << ", group_id: " << group_id << ", modify type: "
                       << modify_type << ", user_cnt: . " << user_cnt;

            if (!IM::BaseDefine::GroupModifyType_IsValid(modify_type)) {
                LOG(ERROR) << "QueryChangeMember, unvalid modify_type";
                char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
                pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
                pHttpConn->Close();
                return;
            }
            CDbAttachData attach_data(ATTACH_TYPE_HANDLE, pHttpConn->GetConnHandle());
            IM::Group::IMGroupChangeMemberReq msg;
            msg.set_user_id(0);
            msg.set_change_type((::IM::BaseDefine::GroupModifyType) modify_type);
            msg.set_group_id(group_id);
            for (uint32_t i = 0; i < user_cnt; i++) {
                uint32_t member_id = post_json_obj["user_id_list"][i].asUInt();
                msg.add_member_id_list(member_id);
            }
            msg.set_attach_data(attach_data.GetBuffer(), attach_data.GetLength());
            CImPdu pdu;
            pdu.SetPBMsg(&msg);
            pdu.SetServiceId(IM::BaseDefine::SID_GROUP);
            pdu.SetCommandId(IM::BaseDefine::CID_GROUP_CHANGE_MEMBER_REQUEST);
            pConn->SendPdu(&pdu);
        }
        catch (std::runtime_error msg) {
            LOG(ERROR) << "parse json data failed.";
            char *response_buf = PackSendResult(HTTP_ERROR_PARMENT, HTTP_ERROR_MSG[1].c_str());
            pHttpConn->Send(response_buf, (uint32_t) strlen(response_buf));
            pHttpConn->Close();
        }
    }

    HTTP_ERROR_CODE
    CHttpQuery::_CheckAuth(const std::string &strAppKey, const uint32_t userId, const std::string &strInterface,
                           const std::string &strIp) {
        return HTTP_ERROR_SUCCESS;
    }

    HTTP_ERROR_CODE
    CHttpQuery::_CheckPermission(const std::string &strAppKey, uint8_t nType, const std::list<uint32_t> &lsToId,
                                 std::string strMsg) {
        strMsg.clear();
        return HTTP_ERROR_SUCCESS;
    }


}