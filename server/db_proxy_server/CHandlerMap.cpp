//
// Created by quran on 2018/8/18.
//

#include <cstdint>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <db_proxy_server/business/DepartAction.hpp>
#include <db_proxy_server/business/Login.hpp>
#include <db_proxy_server/business/UserAction.hpp>
#include <db_proxy_server/business/RecentSession.hpp>
#include <db_proxy_server/business/MessageContent.hpp>
#include <db_proxy_server/business/MessageCounter.hpp>
#include <db_proxy_server/business/GroupAction.hpp>
#include <db_proxy_server/business/FileAction.hpp>
#include "CHandlerMap.hpp"
#include "CProxyTask.hpp"

namespace Flow{
    using namespace IM::BaseDefine;
    CHandlerMap* CHandlerMap::s_handler_instance = NULL;

/**
 *  构造函数
 */
    CHandlerMap::CHandlerMap()
    {

    }

/**
 *  析构函数
 */
    CHandlerMap::~CHandlerMap()
    {

    }

/**
 *  单例
 *
 *  @return 返回指向CHandlerMap的单例指针
 */
    CHandlerMap* CHandlerMap::getInstance()
    {
        if (!s_handler_instance) {
            s_handler_instance = new CHandlerMap();
            s_handler_instance->Init();
        }

        return s_handler_instance;
    }

/**
 *  初始化函数,加载了各种commandId 对应的处理函数
 */
    void CHandlerMap::Init()
    {
        
        // Login validate
        m_handler_map.insert(std::make_pair(uint32_t(CID_OTHER_VALIDATE_REQ), DB_PROXY::doLogin));
        m_handler_map.insert(std::make_pair(uint32_t(CID_LOGIN_REQ_PUSH_SHIELD), DB_PROXY::doPushShield));
        m_handler_map.insert(std::make_pair(uint32_t(CID_LOGIN_REQ_QUERY_PUSH_SHIELD), DB_PROXY::doQueryPushShield));

        // recent session
        m_handler_map.insert(std::make_pair(uint32_t(CID_BUDDY_LIST_RECENT_CONTACT_SESSION_REQUEST), DB_PROXY::getRecentSession));
        m_handler_map.insert(std::make_pair(uint32_t(CID_BUDDY_LIST_REMOVE_SESSION_REQ), DB_PROXY::deleteRecentSession));

        // users
        m_handler_map.insert(std::make_pair(uint32_t(CID_BUDDY_LIST_USER_INFO_REQUEST), DB_PROXY::getUserInfo));
        m_handler_map.insert(std::make_pair(uint32_t(CID_BUDDY_LIST_ALL_USER_REQUEST), DB_PROXY::getChangedUser));
        m_handler_map.insert(std::make_pair(uint32_t(CID_BUDDY_LIST_DEPARTMENT_REQUEST), DB_PROXY::getChgedDepart));
        m_handler_map.insert(std::make_pair(uint32_t(CID_BUDDY_LIST_CHANGE_SIGN_INFO_REQUEST), DB_PROXY::changeUserSignInfo));


        // message content
        m_handler_map.insert(std::make_pair(uint32_t(CID_MSG_DATA), DB_PROXY::sendMessage));
        m_handler_map.insert(std::make_pair(uint32_t(CID_MSG_LIST_REQUEST), DB_PROXY::getMessage));
        m_handler_map.insert(std::make_pair(uint32_t(CID_MSG_UNREAD_CNT_REQUEST), DB_PROXY::getUnreadMsgCounter));
        m_handler_map.insert(std::make_pair(uint32_t(CID_MSG_READ_ACK), DB_PROXY::clearUnreadMsgCounter));
        m_handler_map.insert(std::make_pair(uint32_t(CID_MSG_GET_BY_MSG_ID_REQ), DB_PROXY::getMessageById));
        m_handler_map.insert(std::make_pair(uint32_t(CID_MSG_GET_LATEST_MSG_ID_REQ), DB_PROXY::getLatestMsgId));

        // device token
        m_handler_map.insert(std::make_pair(uint32_t(CID_LOGIN_REQ_DEVICETOKEN), DB_PROXY::setDevicesToken));
        m_handler_map.insert(std::make_pair(uint32_t(CID_OTHER_GET_DEVICE_TOKEN_REQ), DB_PROXY::getDevicesToken));

        //push 推送设置
        m_handler_map.insert(std::make_pair(uint32_t(CID_GROUP_SHIELD_GROUP_REQUEST), DB_PROXY::setGroupPush));
        m_handler_map.insert(std::make_pair(uint32_t(CID_OTHER_GET_SHIELD_REQ), DB_PROXY::getGroupPush));


        // group
        m_handler_map.insert(std::make_pair(uint32_t(CID_GROUP_NORMAL_LIST_REQUEST), DB_PROXY::getNormalGroupList));
        m_handler_map.insert(std::make_pair(uint32_t(CID_GROUP_INFO_REQUEST), DB_PROXY::getGroupInfo));
        m_handler_map.insert(std::make_pair(uint32_t(CID_GROUP_CREATE_REQUEST), DB_PROXY::createGroup));
        m_handler_map.insert(std::make_pair(uint32_t(CID_GROUP_CHANGE_MEMBER_REQUEST), DB_PROXY::modifyMember));


        // file
        m_handler_map.insert(std::make_pair(uint32_t(CID_FILE_HAS_OFFLINE_REQ), DB_PROXY::hasOfflineFile));
        m_handler_map.insert(std::make_pair(uint32_t(CID_FILE_ADD_OFFLINE_REQ), DB_PROXY::addOfflineFile));
        m_handler_map.insert(std::make_pair(uint32_t(CID_FILE_DEL_OFFLINE_REQ), DB_PROXY::delOfflineFile));
    
    }

/**
 *  通过commandId获取处理函数
 *
 *  @param pdu_type commandId
 *
 *  @return 处理函数的函数指针
 */
    pdu_handler_t CHandlerMap::GetHandler(uint32_t pdu_type)
    {
        HandlerMap_t::iterator it = m_handler_map.find(pdu_type);
        if (it != m_handler_map.end()) {
            return it->second;
        } else {
            return NULL;
        }
    }



}