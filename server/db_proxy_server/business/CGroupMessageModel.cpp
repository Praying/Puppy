//
// Created by quran on 2018/8/18.
//

#include <db_proxy_server/CachePool.hpp>
#include <base/common/Util.hpp>
#include <map>
#include <db_proxy_server/DBPool.hpp>
#include <base/common/OSType.hpp>
#include <glog/logging.h>
#include "CGroupMessageModel.hpp"
#include "CAudioModel.hpp"
#include "CGroupModel.hpp"

namespace Flow{
    extern std::string strAudioEnc;

    CGroupMessageModel* CGroupMessageModel::m_pInstance = NULL;

/**
 *  构造函数
 */
    CGroupMessageModel::CGroupMessageModel()
    {

    }

/**
 *  析构函数
 */
    CGroupMessageModel::~CGroupMessageModel()
    {

    }

/**
 *  单例
 *
 *  @return 返回单例指针
 */
    CGroupMessageModel* CGroupMessageModel::getInstance()
    {
        if (!m_pInstance) {
            m_pInstance = new CGroupMessageModel();
        }

        return m_pInstance;
    }

/**
 *  发送群消息接口
 *
 *  @param nRelateId     关系Id
 *  @param nFromId       发送者Id
 *  @param nGroupId      群组Id
 *  @param nMsgType      消息类型
 *  @param nCreateTime   消息创建时间
 *  @param nMsgId        消息Id
 *  @param strMsgContent 消息类容
 *
 *  @return 成功返回true 失败返回false
 */
    bool CGroupMessageModel::sendMessage(uint32_t nFromId, uint32_t nGroupId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime,uint32_t nMsgId, const std::string& strMsgContent)
    {
        bool bRet = false;
        if(CGroupModel::getInstance()->isInGroup(nFromId, nGroupId))
        {
            CDBManager* pDBManager = CDBManager::getInstance();
            CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
            if (pDBConn)
            {
                std::string strTableName = "IMGroupMessage_" + Util::int2string(nGroupId % 8);
                std::string strSql = "insert into " + strTableName + " (`groupId`, `userId`, `msgId`, `content`, `type`, `status`, `updated`, `created`) "\
            "values(?, ?, ?, ?, ?, ?, ?, ?)";

                // 必须在释放连接前delete CPrepareStatement对象，否则有可能多个线程操作mysql对象，会crash
                CPrepareStatement* pStmt = new CPrepareStatement();
                if (pStmt->Init(pDBConn->GetMysql(), strSql))
                {
                    uint32_t nStatus = 0;
                    uint32_t nType = nMsgType;
                    uint32_t index = 0;
                    pStmt->SetParam(index++, nGroupId);
                    pStmt->SetParam(index++, nFromId);
                    pStmt->SetParam(index++, nMsgId);
                    pStmt->SetParam(index++, strMsgContent);
                    pStmt->SetParam(index++, nType);
                    pStmt->SetParam(index++, nStatus);
                    pStmt->SetParam(index++, nCreateTime);
                    pStmt->SetParam(index++, nCreateTime);

                    bool bRet = pStmt->ExecuteUpdate();
                    if (bRet)
                    {
                        CGroupModel::getInstance()->updateGroupChat(nGroupId);
                        incMessageCount(nFromId, nGroupId);
                        clearMessageCount(nFromId, nGroupId);
                    } else {
                        LOG(ERROR)<<"insert message failed: "<< strSql.c_str();
                    }
                }
                delete pStmt;
                pDBManager->RelDBConn(pDBConn);
            }
            else
            {
                LOG(ERROR)<<"no db connection for teamtalk_master";
            }
        }
        else
        {
            LOG(ERROR)<<"not in the group.fromId="<<nFromId<<", groupId="<< nGroupId;
        }
        return bRet;
    }

/**
 *  发送群组语音信息
 *
 *  @param nRelateId   关系Id
 *  @param nFromId     发送者Id
 *  @param nGroupId    群组Id
 *  @param nMsgType    消息类型
 *  @param nCreateTime 消息创建时间
 *  @param nMsgId      消息Id
 *  @param pMsgContent 指向语音类容的指针
 *  @param nMsgLen     语音消息长度
 *
 *  @return 成功返回true，失败返回false
 */
    bool CGroupMessageModel::sendAudioMessage(uint32_t nFromId, uint32_t nGroupId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId, const char* pMsgContent, uint32_t nMsgLen)
    {
        if (nMsgLen <= 4) {
            return false;
        }

        if(!CGroupModel::getInstance()->isInGroup(nFromId, nGroupId))
        {
            LOG(ERROR)<<"not in the group.fromId="<<", groupId="<< nGroupId;
            return false;
        }

        CAudioModel* pAudioModel = CAudioModel::getInstance();
        int nAudioId = pAudioModel->saveAudioInfo(nFromId, nGroupId, nCreateTime, pMsgContent, nMsgLen);

        bool bRet = true;
        if (nAudioId != -1) {
            std::string strMsg = Util::int2string(nAudioId);
            bRet = sendMessage(nFromId, nGroupId, nMsgType, nCreateTime, nMsgId, strMsg);
        } else {
            bRet = false;
        }

        return bRet;
    }

/**
 *  清除群组消息计数
 *
 *  @param nUserId  用户Id
 *  @param nGroupId 群组Id
 *
 *  @return 成功返回true，失败返回false
 */
    bool CGroupMessageModel::clearMessageCount(uint32_t nUserId, uint32_t nGroupId)
    {
        bool bRet = false;
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn)
        {
            std::string strGroupKey = Util::int2string(nGroupId) + GROUP_TOTAL_MSG_COUNTER_REDIS_KEY_SUFFIX;
            std::map<std::string, std::string> mapGroupCount;
            bool bRet = pCacheConn->hgetAll(strGroupKey, mapGroupCount);
            pCacheManager->RelCacheConn(pCacheConn);
            if(bRet)
            {
                std::string strUserKey = Util::int2string(nUserId) + "_" + Util::int2string(nGroupId) + GROUP_USER_MSG_COUNTER_REDIS_KEY_SUFFIX;
                std::string strReply = pCacheConn->hmset(strUserKey, mapGroupCount);
                if(strReply.empty())
                {
                    LOG(ERROR)<<"hmset %s failed !"<< strUserKey.c_str();
                }
                else
                {
                    bRet = true;
                }
            }
            else
            {
                LOG(ERROR)<<"hgetAll %s failed !"<< strGroupKey.c_str();
            }
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
        return bRet;
    }

/**
 *  增加群消息计数
 *
 *  @param nUserId  用户Id
 *  @param nGroupId 群组Id
 *
 *  @return 成功返回true，失败返回false
 */
    bool CGroupMessageModel::incMessageCount(uint32_t nUserId, uint32_t nGroupId)
    {
        bool bRet = false;
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn)
        {
            std::string strGroupKey = Util::int2string(nGroupId) + GROUP_TOTAL_MSG_COUNTER_REDIS_KEY_SUFFIX;
            pCacheConn->hincrBy(strGroupKey, GROUP_COUNTER_SUBKEY_COUNTER_FIELD, 1);
            std::map<std::string, std::string> mapGroupCount;
            bool bRet = pCacheConn->hgetAll(strGroupKey, mapGroupCount);
            if(bRet)
            {
                std::string strUserKey = Util::int2string(nUserId) + "_" + Util::int2string(nGroupId) + GROUP_USER_MSG_COUNTER_REDIS_KEY_SUFFIX;
                std::string strReply = pCacheConn->hmset(strUserKey, mapGroupCount);
                if(!strReply.empty())
                {
                    bRet = true;
                }
                else
                {
                    LOG(ERROR)<<"hmset "<<strUserKey<<" failed !";
                }
            }
            else
            {
                LOG(ERROR)<<"hgetAll "<<strGroupKey<<" failed!";
            }
            pCacheManager->RelCacheConn(pCacheConn);
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
        return bRet;
    }

/**
 *  获取群组消息列表
 *
 *  @param nUserId  用户Id
 *  @param nGroupId 群组Id
 *  @param nMsgId   开始的msgId(最新的msgId)
 *  @param nMsgCnt  获取的长度
 *  @param lsMsg    消息列表
 */
    void CGroupMessageModel::getMessage(uint32_t nUserId, uint32_t nGroupId, uint32_t nMsgId, uint32_t nMsgCnt, std::list<IM::BaseDefine::MsgInfo>& lsMsg)
    {
        //根据 count 和 lastId 获取信息
        std::string strTableName = "IMGroupMessage_" + Util::int2string(nGroupId % 8);

        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            uint32_t nUpdated = CGroupModel::getInstance()->getUserJoinTime(nGroupId, nUserId);
            //如果nMsgId 为0 表示客户端想拉取最新的nMsgCnt条消息
            std::string strSql;
            if(nMsgId == 0)
            {
                strSql = "select * from " + strTableName + " where groupId = " + Util::int2string(nGroupId) + " and status = 0 and created>="+ Util::int2string(nUpdated) + " order by created desc, id desc limit " + Util::int2string(nMsgCnt);
            }else {
                strSql = "select * from " + strTableName + " where groupId = " + Util::int2string(nGroupId) + " and msgId<=" + Util::int2string(nMsgId) + " and status = 0 and created>="+ Util::int2string(nUpdated) + " order by created desc, id desc limit " + Util::int2string(nMsgCnt);
            }

            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if (pResultSet)
            {
                std::map<uint32_t, IM::BaseDefine::MsgInfo> mapAudioMsg;
                while(pResultSet->Next())
                {
                    IM::BaseDefine::MsgInfo msg;
                    msg.set_msg_id(pResultSet->GetInt("msgId"));
                    msg.set_from_session_id(pResultSet->GetInt("userId"));
                    msg.set_create_time(pResultSet->GetInt("created"));
                    IM::BaseDefine::MsgType nMsgType = IM::BaseDefine::MsgType(pResultSet->GetInt("type"));
                    if(IM::BaseDefine::MsgType_IsValid(nMsgType))
                    {
                        msg.set_msg_type(nMsgType);
                        msg.set_msg_data(pResultSet->GetString("content"));
                        lsMsg.push_back(msg);
                    }
                    else
                    {
                        LOG(ERROR)<<"invalid msgType. userId="<< nUserId<<", groupId="<<nGroupId<<", msgType="<<nMsgType;

                    }
                }
                delete pResultSet;
            }
            else
            {
                LOG(ERROR)<<"no result set for sql: "<< strSql;
            }
            pDBManager->RelDBConn(pDBConn);
            if (!lsMsg.empty()) {
                CAudioModel::getInstance()->readAudios(lsMsg);
            }
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_slave";
        }
    }

/**
 *  获取用户群未读消息计数
 *
 *  @param nUserId       用户Id
 *  @param nTotalCnt     总条数
 *  @param lsUnreadCount 每个会话的未读信息包含了条数，最后一个消息的Id，最后一个消息的类型，最后一个消息的类容
 */
    void CGroupMessageModel::getUnreadMsgCount(uint32_t nUserId, uint32_t &nTotalCnt, std::list<IM::BaseDefine::UnreadInfo>& lsUnreadCount)
    {
        std::list<uint32_t> lsGroupId;
        CGroupModel::getInstance()->getUserGroupIds(nUserId, lsGroupId, 0);
        uint32_t nCount = 0;

        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn)
        {
            for(auto it=lsGroupId.begin(); it!=lsGroupId.end(); ++it)
            {
                uint32_t nGroupId = *it;
                std::string strGroupKey = Util::int2string(nGroupId) + GROUP_TOTAL_MSG_COUNTER_REDIS_KEY_SUFFIX;
                std::string strGroupCnt = pCacheConn->hget(strGroupKey, GROUP_COUNTER_SUBKEY_COUNTER_FIELD);
                if(strGroupCnt.empty())
                {
//                LOG(ERROR)<<"hget %s : count failed !", strGroupKey.c_str());
                    continue;
                }
                uint32_t nGroupCnt = (uint32_t)(atoi(strGroupCnt.c_str()));

                std::string strUserKey = Util::int2string(nUserId) + "_" + Util::int2string(nGroupId) + GROUP_USER_MSG_COUNTER_REDIS_KEY_SUFFIX;
                std::string strUserCnt = pCacheConn->hget(strUserKey, GROUP_COUNTER_SUBKEY_COUNTER_FIELD);

                uint32_t nUserCnt = ( strUserCnt.empty() ? 0 : ((uint32_t)atoi(strUserCnt.c_str())) );
                if(nGroupCnt >= nUserCnt) {
                    nCount = nGroupCnt - nUserCnt;
                }
                if(nCount > 0)
                {
                    IM::BaseDefine::UnreadInfo cUnreadInfo;
                    cUnreadInfo.set_session_id(nGroupId);
                    cUnreadInfo.set_session_type(IM::BaseDefine::SESSION_TYPE_GROUP);
                    cUnreadInfo.set_unread_cnt(nCount);
                    nTotalCnt += nCount;
                    std::string strMsgData;
                    uint32_t nMsgId;
                    IM::BaseDefine::MsgType nType;
                    uint32_t nFromId;
                    getLastMsg(nGroupId, nMsgId, strMsgData, nType, nFromId);
                    if(IM::BaseDefine::MsgType_IsValid(nType))
                    {
                        cUnreadInfo.set_latest_msg_id(nMsgId);
                        cUnreadInfo.set_latest_msg_data(strMsgData);
                        cUnreadInfo.set_latest_msg_type(nType);
                        cUnreadInfo.set_latest_msg_from_user_id(nFromId);
                        lsUnreadCount.push_back(cUnreadInfo);
                    }
                    else
                    {
                        LOG(ERROR)<<"invalid msgType. userId="<<nUserId<<", groupId="<<nGroupId<<", msgType="<<nType<<", msgId=" <<nMsgId;
                    }
                }
            }
            pCacheManager->RelCacheConn(pCacheConn);
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
    }

/**
 *  获取一个群组的msgId，自增，通过redis控制
 *
 *  @param nGroupId 群Id
 *
 *  @return 返回msgId
 */
    uint32_t CGroupMessageModel::getMsgId(uint32_t nGroupId)
    {
        uint32_t nMsgId = 0;
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if(pCacheConn)
        {
            // TODO
            std::string strKey = "group_msg_id_" + Util::int2string(nGroupId);
            nMsgId = pCacheConn->incrBy(strKey, 1);
            pCacheManager->RelCacheConn(pCacheConn);
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
        return nMsgId;
    }

/**
 *  获取一个群的最后一条消息
 *
 *  @param nGroupId   群Id
 *  @param nMsgId     最后一条消息的msgId,引用
 *  @param strMsgData 最后一条消息的内容,引用
 *  @param nMsgType   最后一条消息的类型,引用
 */
    void CGroupMessageModel::getLastMsg(uint32_t nGroupId, uint32_t &nMsgId, std::string &strMsgData, IM::BaseDefine::MsgType &nMsgType, uint32_t& nFromId)
    {
        std::string strTableName = "IMGroupMessage_" + Util::int2string(nGroupId % 8);

        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            std::string strSql = "select msgId, type,userId, content from " + strTableName + " where groupId = " + Util::int2string(nGroupId) + " and status = 0 order by created desc, id desc limit 1";

            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if (pResultSet)
            {
                while(pResultSet->Next()) {
                    nMsgId = pResultSet->GetInt("msgId");
                    nMsgType = IM::BaseDefine::MsgType(pResultSet->GetInt("type"));
                    nFromId = pResultSet->GetInt("userId");
                    if(nMsgType == IM::BaseDefine::MSG_TYPE_GROUP_AUDIO)
                    {
                        // "[语音]"加密后的字符串
                        strMsgData = strAudioEnc;
                    }
                    else
                    {
                        strMsgData = pResultSet->GetString("content");
                    }
                }
                delete pResultSet;
            }
            else
            {
                LOG(ERROR)<<"no result set for sql: " << strSql.c_str();
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_slave";
        }
    }

/**
 *  获取某个用户所有群的所有未读计数之和
 *
 *  @param nUserId   用户Id
 *  @param nTotalCnt 未读计数之后,引用
 */
    void CGroupMessageModel::getUnReadCntAll(uint32_t nUserId, uint32_t &nTotalCnt)
    {
        std::list<uint32_t> lsGroupId;
        CGroupModel::getInstance()->getUserGroupIds(nUserId, lsGroupId, 0);
        uint32_t nCount = 0;

        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn)
        {
            for(auto it=lsGroupId.begin(); it!=lsGroupId.end(); ++it)
            {
                uint32_t nGroupId = *it;
                std::string strGroupKey = Util::int2string(nGroupId) + GROUP_TOTAL_MSG_COUNTER_REDIS_KEY_SUFFIX;
                std::string strGroupCnt = pCacheConn->hget(strGroupKey, GROUP_COUNTER_SUBKEY_COUNTER_FIELD);
                if(strGroupCnt.empty())
                {
//                LOG(ERROR)<<"hget %s : count failed !", strGroupKey.c_str());
                    continue;
                }
                uint32_t nGroupCnt = (uint32_t)(atoi(strGroupCnt.c_str()));

                std::string strUserKey = Util::int2string(nUserId) + "_" + Util::int2string(nGroupId) + GROUP_USER_MSG_COUNTER_REDIS_KEY_SUFFIX;
                std::string strUserCnt = pCacheConn->hget(strUserKey, GROUP_COUNTER_SUBKEY_COUNTER_FIELD);

                uint32_t nUserCnt = ( strUserCnt.empty() ? 0 : ((uint32_t)atoi(strUserCnt.c_str())) );
                if(nGroupCnt >= nUserCnt) {
                    nCount = nGroupCnt - nUserCnt;
                }
                if(nCount > 0)
                {
                    nTotalCnt += nCount;
                }
            }
            pCacheManager->RelCacheConn(pCacheConn);
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
    }

    void CGroupMessageModel::getMsgByMsgId(uint32_t nUserId, uint32_t nGroupId, const std::list<uint32_t> &lsMsgId, std::list<IM::BaseDefine::MsgInfo> &lsMsg)
    {
        if(!lsMsgId.empty())
        {
            if (CGroupModel::getInstance()->isInGroup(nUserId, nGroupId))
            {
                CDBManager* pDBManager = CDBManager::getInstance();
                CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
                if (pDBConn)
                {
                    std::string strTableName = "IMGroupMessage_" + Util::int2string(nGroupId % 8);
                    uint32_t nUpdated = CGroupModel::getInstance()->getUserJoinTime(nGroupId, nUserId);
                    std::string strClause ;
                    bool bFirst = true;
                    for(auto it= lsMsgId.begin(); it!=lsMsgId.end();++it)
                    {
                        if (bFirst) {
                            bFirst = false;
                            strClause = Util::int2string(*it);
                        }
                        else
                        {
                            strClause += ("," + Util::int2string(*it));
                        }
                    }

                    std::string strSql = "select * from " + strTableName + " where groupId=" + Util::int2string(nGroupId) + " and msgId in (" + strClause + ") and status=0 and created >= " + Util::int2string(nUpdated) + " order by created desc, id desc limit 100";
                    CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
                    if (pResultSet)
                    {
                        while (pResultSet->Next())
                        {
                            IM::BaseDefine::MsgInfo msg;
                            msg.set_msg_id(pResultSet->GetInt("msgId"));
                            msg.set_from_session_id(pResultSet->GetInt("userId"));
                            msg.set_create_time(pResultSet->GetInt("created"));
                            IM::BaseDefine::MsgType nMsgType = IM::BaseDefine::MsgType(pResultSet->GetInt("type"));
                            if(IM::BaseDefine::MsgType_IsValid(nMsgType))
                            {
                                msg.set_msg_type(nMsgType);
                                msg.set_msg_data(pResultSet->GetString("content"));
                                lsMsg.push_back(msg);
                            }
                            else
                            {
                                LOG(ERROR)<<"invalid msgType. userId="<<nUserId<<", groupId="<<nGroupId<<", msgType="<<nMsgType;
                            }
                        }
                        delete pResultSet;
                    }
                    else
                    {
                        LOG(ERROR)<<"no result set for sql:" << strSql.c_str();
                    }
                    pDBManager->RelDBConn(pDBConn);
                    if(!lsMsg.empty())
                    {
                        CAudioModel::getInstance()->readAudios(lsMsg);
                    }
                }
                else
                {
                    LOG(ERROR)<<"no db connection for teamtalk_slave";
                }
            }
            else
            {
                LOG(ERROR)<<nUserId<<" is not in group:"<< nGroupId;
            }
        }
        else
        {
            LOG(ERROR)<<"msgId is empty.";
        }
    }

    bool CGroupMessageModel::resetMsgId(uint32_t nGroupId)
    {
        bool bRet = false;
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if(pCacheConn)
        {
            std::string strKey = "group_msg_id_" + Util::int2string(nGroupId);
            std::string strValue = "0";
            std::string strReply = pCacheConn->set(strKey, strValue);
            if(strReply == strValue)
            {
                bRet = true;
            }
            pCacheManager->RelCacheConn(pCacheConn);
        }
        return bRet;
    }
}