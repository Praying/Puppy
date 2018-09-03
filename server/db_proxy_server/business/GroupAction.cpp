//
// Created by quran on 2018/9/4.
//
#include <base/proto/ProtosGen/IM.Server.pb.h>
#include <list>
#include <map>
#include <glog/logging.h>
#include <glog/raw_logging.h>
#include <db_proxy_server/CProxyConn.hpp>
#include <base/proto/ProtosGen/IM.Group.pb.h>
#include <base/common/PublicDefine.hpp>
#include "GroupAction.hpp"
#include "CGroupModel.hpp"

namespace Flow::DB_PROXY{
    /**
    *  创建群组
    *
    *  @param pPdu      收到的packet包指针
    *  @param conn_uuid 该包过来的socket 描述符
    */
    void createGroup(CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Group::IMGroupCreateReq msg;
        IM::Group::IMGroupCreateRsp msgResp;
        if(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()))
        {
            CImPdu* pPduRes = new CImPdu;

            uint32_t nUserId = msg.user_id();
            std::string strGroupName = msg.group_name();
            IM::BaseDefine::GroupType nGroupType = msg.group_type();
            if(IM::BaseDefine::GroupType_IsValid(nGroupType))
            {
                std::string strGroupAvatar = msg.group_avatar();
                std::set<uint32_t> setMember;
                uint32_t nMemberCnt = msg.member_id_list_size();
                for(uint32_t i=0; i<nMemberCnt; ++i)
                {
                    uint32_t nUserId = msg.member_id_list(i);
                    setMember.insert(nUserId);
                }
                RAW_LOG(INFO,"createGroup.%d create %s, userCnt=%u", nUserId, strGroupName.c_str(), setMember.size());

                uint32_t nGroupId = CGroupModel::getInstance()->createGroup(nUserId, strGroupName, strGroupAvatar, nGroupType, setMember);
                msgResp.set_user_id(nUserId);
                msgResp.set_group_name(strGroupName);
                for(auto it=setMember.begin(); it!=setMember.end();++it)
                {
                    msgResp.add_user_id_list(*it);
                }
                if(nGroupId != INVALID_VALUE)
                {
                    msgResp.set_result_code(0);
                    msgResp.set_group_id(nGroupId);
                }
                else
                {
                    msgResp.set_result_code(1);
                }


                RAW_LOG(INFO,"createGroup.%d create %s, userCnt=%u, result:%d", nUserId, strGroupName.c_str(), setMember.size(), msgResp.result_code());

                msgResp.set_attach_data(msg.attach_data());
                pPduRes->SetPBMsg(&msgResp);
                pPduRes->SetSeqNum(pPdu->GetSeqNum());
                pPduRes->SetServiceId(IM::BaseDefine::SID_GROUP);
                pPduRes->SetCommandId(IM::BaseDefine::CID_GROUP_CREATE_RESPONSE);
                CProxyConn::AddResponsePdu(conn_uuid, pPduRes);
            }
            else
            {
                RAW_LOG(INFO,"invalid group type.userId=%u, groupType=%u, groupName=%s", nUserId, nGroupType, strGroupName.c_str());
            }
        }
        else
        {
            RAW_LOG(INFO,"parse pb failed");
        }
    }

    /**
     *  获取正式群列表
     *
     *  @param pPdu      收到的packet包指针
     *  @param conn_uuid 该包过来的socket 描述符
     */
    void getNormalGroupList(CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Group::IMNormalGroupListReq msg;
        IM::Group::IMNormalGroupListRsp msgResp;
        if(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()))
        {
            CImPdu* pPduRes = new CImPdu;

            uint32_t nUserId = msg.user_id();

            std::list<IM::BaseDefine::GroupVersionInfo> lsGroup;
            CGroupModel::getInstance()->getUserGroup(nUserId, lsGroup, IM::BaseDefine::GROUP_TYPE_NORMAL);
            msgResp.set_user_id(nUserId);
            for(auto it=lsGroup.begin(); it!=lsGroup.end(); ++it)
            {
                IM::BaseDefine::GroupVersionInfo* pGroupVersion = msgResp.add_group_version_list();
                pGroupVersion->set_group_id(it->group_id());
                pGroupVersion->set_version(it->version());
            }

            RAW_LOG(INFO,"getNormalGroupList. userId=%u, count=%d", nUserId, msgResp.group_version_list_size());

            msgResp.set_attach_data(msg.attach_data());
            pPduRes->SetPBMsg(&msgResp);
            pPduRes->SetSeqNum(pPdu->GetSeqNum());
            pPduRes->SetServiceId(IM::BaseDefine::SID_GROUP);
            pPduRes->SetCommandId(IM::BaseDefine::CID_GROUP_NORMAL_LIST_RESPONSE);
            CProxyConn::AddResponsePdu(conn_uuid, pPduRes);
        }
        else
        {
            RAW_LOG(INFO,"parse pb failed");
        }
    }

    /**
     *  获取群信息
     *
     *  @param pPdu      收到的packet包指针
     *  @param conn_uuid 该包过来的socket 描述符
     */
    void getGroupInfo(CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Group::IMGroupInfoListReq msg;
        IM::Group::IMGroupInfoListRsp msgResp;
        if(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()))
        {
            CImPdu* pPduRes = new CImPdu;
            uint32_t nUserId = msg.user_id();
            uint32_t nGroupCnt = msg.group_version_list_size();

            std::map<uint32_t, IM::BaseDefine::GroupVersionInfo> mapGroupId;
            for(uint32_t i=0; i<nGroupCnt; ++i)
            {
                IM::BaseDefine::GroupVersionInfo groupInfo = msg.group_version_list(i);
                if(CGroupModel::getInstance()->isValidateGroupId(groupInfo.group_id()))
                {
                    mapGroupId[groupInfo.group_id()] = groupInfo;
                }
            }
            std::list<IM::BaseDefine::GroupInfo> lsGroupInfo;
            CGroupModel::getInstance()->getGroupInfo(mapGroupId, lsGroupInfo);

            msgResp.set_user_id(nUserId);
            for(auto it=lsGroupInfo.begin(); it!=lsGroupInfo.end(); ++it)
            {
                IM::BaseDefine::GroupInfo* pGroupInfo = msgResp.add_group_info_list();
                //            *pGroupInfo = *it;
                pGroupInfo->set_group_id(it->group_id());
                pGroupInfo->set_version(it->version());
                pGroupInfo->set_group_name(it->group_name());
                pGroupInfo->set_group_avatar(it->group_avatar());
                pGroupInfo->set_group_creator_id(it->group_creator_id());
                pGroupInfo->set_group_type(it->group_type());
                pGroupInfo->set_shield_status(it->shield_status());
                uint32_t nGroupMemberCnt = it->group_member_list_size();
                for (uint32_t i=0; i<nGroupMemberCnt; ++i) {
                    uint32_t userId = it->group_member_list(i);
                    pGroupInfo->add_group_member_list(userId);
                }
            }

            RAW_LOG(INFO,"userId=%u, requestCount=%u", nUserId, nGroupCnt);

            msgResp.set_attach_data(msg.attach_data());
            pPduRes->SetPBMsg(&msgResp);
            pPduRes->SetSeqNum(pPdu->GetSeqNum());
            pPduRes->SetServiceId(IM::BaseDefine::SID_GROUP);
            pPduRes->SetCommandId(IM::BaseDefine::CID_GROUP_INFO_RESPONSE);
            CProxyConn::AddResponsePdu(conn_uuid, pPduRes);
        }
        else
        {
            RAW_LOG(INFO,"parse pb failed");
        }
    }
    /**
     *  修改群成员，增加或删除
     *
     *  @param pPdu      收到的packet包指针
     *  @param conn_uuid 该包过来的socket 描述符
     */
    void modifyMember(CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Group::IMGroupChangeMemberReq msg;
        IM::Group::IMGroupChangeMemberRsp msgResp;
        if(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()))
        {
            uint32_t nUserId = msg.user_id();
            uint32_t nGroupId = msg.group_id();
            IM::BaseDefine::GroupModifyType nType = msg.change_type();
            if (IM::BaseDefine::GroupModifyType_IsValid(nType) &&
                CGroupModel::getInstance()->isValidateGroupId(nGroupId)) {

                CImPdu* pPduRes = new CImPdu;

                uint32_t nCnt = msg.member_id_list_size();
                std::set<uint32_t> setUserId;
                for(uint32_t i=0; i<nCnt;++i)
                {
                    setUserId.insert(msg.member_id_list(i));
                }
                std::list<uint32_t> lsCurUserId;
                bool bRet = CGroupModel::getInstance()->modifyGroupMember(nUserId, nGroupId, nType, setUserId, lsCurUserId);
                msgResp.set_user_id(nUserId);
                msgResp.set_group_id(nGroupId);
                msgResp.set_change_type(nType);
                msgResp.set_result_code(bRet?0:1);
                if(bRet)
                {
                    for(auto it=setUserId.begin(); it!=setUserId.end(); ++it)
                    {
                        msgResp.add_chg_user_id_list(*it);
                    }

                    for(auto it=lsCurUserId.begin(); it!=lsCurUserId.end(); ++it)
                    {
                        msgResp.add_cur_user_id_list(*it);
                    }
                }
                RAW_LOG(INFO,"userId=%u, groupId=%u, result=%u, changeCount:%u, currentCount=%u",nUserId, nGroupId,  bRet?0:1, msgResp.chg_user_id_list_size(), msgResp.cur_user_id_list_size());
                msgResp.set_attach_data(msg.attach_data());
                pPduRes->SetPBMsg(&msgResp);
                pPduRes->SetSeqNum(pPdu->GetSeqNum());
                pPduRes->SetServiceId(IM::BaseDefine::SID_GROUP);
                pPduRes->SetCommandId(IM::BaseDefine::CID_GROUP_CHANGE_MEMBER_RESPONSE);
                CProxyConn::AddResponsePdu(conn_uuid, pPduRes);
            }
            else
            {
                RAW_LOG(INFO,"invalid groupModifyType or groupId. userId=%u, groupId=%u, groupModifyType=%u", nUserId, nGroupId, nType);
            }

        }
        else
        {
            RAW_LOG(INFO,"parse pb failed");
        }
    }

    /**
     *  设置群组信息推送，屏蔽或者取消屏蔽
     *
     *  @param pPdu      收到的packet包指针
     *  @param conn_uuid 该包过来的socket 描述符
     */
    void setGroupPush(CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Group::IMGroupShieldReq msg;
        IM::Group::IMGroupShieldRsp msgResp;
        if(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()))
        {
            uint32_t nUserId = msg.user_id();
            uint32_t nGroupId = msg.group_id();
            uint32_t nStatus = msg.shield_status();
            if(CGroupModel::getInstance()->isValidateGroupId(nGroupId))
            {

                CImPdu* pPduRes = new CImPdu;
                bool bRet = CGroupModel::getInstance()->setPush(nUserId, nGroupId, IM_GROUP_SETTING_PUSH, nStatus);

                msgResp.set_user_id(nUserId);
                msgResp.set_group_id(nGroupId);
                msgResp.set_result_code(bRet?0:1);

                RAW_LOG(INFO,"userId=%u, groupId=%u, result=%u", nUserId, nGroupId, msgResp.result_code());

                msgResp.set_attach_data(msg.attach_data());
                pPduRes->SetPBMsg(&msgResp);
                pPduRes->SetSeqNum(pPdu->GetSeqNum());
                pPduRes->SetServiceId(IM::BaseDefine::SID_GROUP);
                pPduRes->SetCommandId(IM::BaseDefine::CID_GROUP_SHIELD_GROUP_RESPONSE);
                CProxyConn::AddResponsePdu(conn_uuid, pPduRes);
            }
            else
            {
                RAW_LOG(INFO,"Invalid group.userId=%u, groupId=%u", nUserId, nGroupId);
            }
        }
        else
        {
            RAW_LOG(INFO,"parse pb failed");
        }
    }

    /**
     *  获取一个群的推送设置
     *
     *  @param pPdu      收到的packet包指针
     *  @param conn_uuid 该包过来的socket 描述符
     */
    void getGroupPush(CImPdu* pPdu, uint32_t conn_uuid)
    {
        IM::Server::IMGroupGetShieldReq msg;
        IM::Server::IMGroupGetShieldRsp msgResp;
        if(msg.ParseFromArray(pPdu->GetBodyData(), pPdu->GetBodyLength()))
        {
            uint32_t nGroupId = msg.group_id();
            uint32_t nUserCnt = msg.user_id_size();
            if(CGroupModel::getInstance()->isValidateGroupId(nGroupId))
            {
                CImPdu* pPduRes = new CImPdu;
                std::list<uint32_t> lsUser;
                for(uint32_t i=0; i<nUserCnt; ++i)
                {
                    lsUser.push_back(msg.user_id(i));
                }
                std::list<IM::BaseDefine::ShieldStatus> lsPush;
                CGroupModel::getInstance()->getPush(nGroupId, lsUser, lsPush);

                msgResp.set_group_id(nGroupId);
                for (auto it=lsPush.begin(); it!=lsPush.end(); ++it) {
                    IM::BaseDefine::ShieldStatus* pStatus = msgResp.add_shield_status_list();
                    //            *pStatus = *it;
                    pStatus->set_user_id(it->user_id());
                    pStatus->set_group_id(it->group_id());
                    pStatus->set_shield_status(it->shield_status());
                }

                RAW_LOG(INFO,"groupId=%u, count=%u", nGroupId, nUserCnt);

                msgResp.set_attach_data(msg.attach_data());
                pPduRes->SetPBMsg(&msgResp);
                pPduRes->SetSeqNum(pPdu->GetSeqNum());
                pPduRes->SetServiceId(IM::BaseDefine::SID_OTHER);
                pPduRes->SetCommandId(IM::BaseDefine::CID_OTHER_GET_SHIELD_RSP);
                CProxyConn::AddResponsePdu(conn_uuid, pPduRes);
            }
            else
            {
                RAW_LOG(INFO,"Invalid groupId. nGroupId=%u", nGroupId);
            }
        }
        else
        {
            RAW_LOG(INFO,"parse pb failed");
        }
    }
}
