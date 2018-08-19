//
// Created by quran on 2018/8/18.
//

#include <db_proxy_server/CachePool.hpp>
#include <db_proxy_server/DBPool.hpp>
#include <base/common/Util.hpp>
#include "MessageModel.hpp"
#include "CAudioModel.hpp"
#include "CRelationModel.hpp"
#include <string>
#include <list>
#include <glog/logging.h>
#include <base/common/OSType.hpp>

namespace Flow{
    using namespace std;
    using namespace Flow::Util;
    CMessageModel* CMessageModel::m_pInstance = NULL;
    extern std::string strAudioEnc;

    CMessageModel::CMessageModel()
    {

    }

    CMessageModel::~CMessageModel()
    {

    }

    CMessageModel* CMessageModel::getInstance()
    {
        if (!m_pInstance) {
            m_pInstance = new CMessageModel();
        }

        return m_pInstance;
    }

    void CMessageModel::getMessage(uint32_t nUserId, uint32_t nPeerId, uint32_t nMsgId,
                                   uint32_t nMsgCnt, std::list<IM::BaseDefine::MsgInfo>& lsMsg)
    {
        uint32_t nRelateId = CRelationModel::getInstance()->getRelationId(nUserId, nPeerId, false);
        if (nRelateId != INVALID_VALUE)
        {
            CDBManager* pDBManager = CDBManager::getInstance();
            CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
            if (pDBConn)
            {
                std::string strTableName = "IMMessage_" + int2string(nRelateId % 8);
                std::string strSql;
                if (nMsgId == 0) {
                    strSql = "select * from " + strTableName + " force index (idx_relateId_status_created) where relateId= " + int2string(nRelateId) + " and status = 0 order by created desc, id desc limit " + int2string(nMsgCnt);
                }
                else
                {
                    strSql = "select * from " + strTableName + " force index (idx_relateId_status_created) where relateId= " + int2string(nRelateId) + " and status = 0 and msgId <=" + int2string(nMsgId)+ " order by created desc, id desc limit " + int2string(nMsgCnt);
                }
                CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
                if (pResultSet)
                {
                    while (pResultSet->Next())
                    {
                        IM::BaseDefine::MsgInfo cMsg;
                        cMsg.set_msg_id(pResultSet->GetInt("msgId"));
                        cMsg.set_from_session_id(pResultSet->GetInt("fromId"));
                        cMsg.set_create_time(pResultSet->GetInt("created"));
                        IM::BaseDefine::MsgType nMsgType = IM::BaseDefine::MsgType(pResultSet->GetInt("type"));
                        if(IM::BaseDefine::MsgType_IsValid(nMsgType))
                        {
                            cMsg.set_msg_type(nMsgType);
                            cMsg.set_msg_data(pResultSet->GetString("content"));
                            lsMsg.push_back(cMsg);
                        }
                        else
                        {
                            //LOG(ERROR)<<"invalid msgType. userId=%u, peerId=%u, msgId=%u, msgCnt=%u, msgType=%u", nUserId, nPeerId, nMsgId, nMsgCnt, nMsgType);
                            LOG(ERROR)<<"invalid msgType. userId="<<nUserId<<",peerId="<<nPeerId<<",msgId="<<nMsgId<<"msgCnt="<<nMsgCnt<<"msgType="<<nMsgType;
                        }
                    }
                    delete pResultSet;
                }
                else
                {
                    LOG(ERROR)<<"no result set: "<<strSql.c_str();
                }
                pDBManager->RelDBConn(pDBConn);
                if (!lsMsg.empty())
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
            LOG(ERROR)<<"no relation between "<<nUserId<<"and"<<nPeerId;
        }
    }

/*
 * IMMessage 分表
 * AddFriendShip()
 * if nFromId or nToId is ShopEmployee
 * GetShopId
 * Insert into IMMessage_ShopId%8
 */
    bool CMessageModel::sendMessage(uint32_t nRelateId, uint32_t nFromId, uint32_t nToId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId, std::string& strMsgContent)
    {
        bool bRet =false;
        if (nFromId == 0 || nToId == 0) {
            LOG(ERROR)<<"invalied userId. "<<nFromId<<"->"<< nToId;
            return bRet;
        }

        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            std::string strTableName = "IMMessage_" + int2string(nRelateId % 8);
            std::string strSql = "insert into " + strTableName + " (`relateId`, `fromId`, `toId`, `msgId`, `content`, `status`, `type`, `created`, `updated`) values(?, ?, ?, ?, ?, ?, ?, ?, ?)";
            // 必须在释放连接前delete CPrepareStatement对象，否则有可能多个线程操作mysql对象，会crash
            CPrepareStatement* pStmt = new CPrepareStatement();
            if (pStmt->Init(pDBConn->GetMysql(), strSql))
            {
                uint32_t nStatus = 0;
                uint32_t nType = nMsgType;
                uint32_t index = 0;
                pStmt->SetParam(index++, nRelateId);
                pStmt->SetParam(index++, nFromId);
                pStmt->SetParam(index++, nToId);
                pStmt->SetParam(index++, nMsgId);
                pStmt->SetParam(index++, strMsgContent);
                pStmt->SetParam(index++, nStatus);
                pStmt->SetParam(index++, nType);
                pStmt->SetParam(index++, nCreateTime);
                pStmt->SetParam(index++, nCreateTime);
                bRet = pStmt->ExecuteUpdate();
            }
            delete pStmt;
            pDBManager->RelDBConn(pDBConn);
            if (bRet)
            {
                uint32_t nNow = (uint32_t) time(NULL);
                incMsgCount(nFromId, nToId);
            }
            else
            {
                LOG(ERROR)<<"insert message failed: %s", strSql.c_str();
            }
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_master";
        }
        return bRet;
    }

    bool CMessageModel::sendAudioMessage(uint32_t nRelateId, uint32_t nFromId, uint32_t nToId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId, const char* pMsgContent, uint32_t nMsgLen)
    {
        if (nMsgLen <= 4) {
            return false;
        }

        CAudioModel* pAudioModel = CAudioModel::getInstance();
        int nAudioId = pAudioModel->saveAudioInfo(nFromId, nToId, nCreateTime, pMsgContent, nMsgLen);

        bool bRet = true;
        if (nAudioId != -1) {
            std::string strMsg = int2string(nAudioId);
            bRet = sendMessage(nRelateId, nFromId, nToId, nMsgType, nCreateTime, nMsgId, strMsg);
        } else {
            bRet = false;
        }

        return bRet;
    }

    void CMessageModel::incMsgCount(uint32_t nFromId, uint32_t nToId)
    {
        CacheManager* pCacheManager = CacheManager::getInstance();
        // increase message count
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn) {
            pCacheConn->hincrBy("unread_" + int2string(nToId), int2string(nFromId), 1);
            pCacheManager->RelCacheConn(pCacheConn);
        } else {
            LOG(ERROR)<<"no cache connection to increase unread count: "<< nFromId<<"->"<<nToId;
        }
    }

    void CMessageModel::getUnreadMsgCount(uint32_t nUserId, uint32_t &nTotalCnt, std::list<IM::BaseDefine::UnreadInfo>& lsUnreadCount)
    {
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn)
        {
            std::map<std::string, std::string> mapUnread;
            std::string strKey = "unread_" + int2string(nUserId);
            bool bRet = pCacheConn->hgetAll(strKey, mapUnread);
            pCacheManager->RelCacheConn(pCacheConn);
            if(bRet)
            {
                IM::BaseDefine::UnreadInfo cUnreadInfo;
                for (auto it = mapUnread.begin(); it != mapUnread.end(); it++) {
                    cUnreadInfo.set_session_id(std::stoi(it->first.c_str()));
                    cUnreadInfo.set_unread_cnt(std::stoi(it->second.c_str()));
                    cUnreadInfo.set_session_type(IM::BaseDefine::SESSION_TYPE_SINGLE);
                    uint32_t nMsgId = 0;
                    std::string strMsgData;
                    IM::BaseDefine::MsgType nMsgType;
                    getLastMsg(cUnreadInfo.session_id(), nUserId, nMsgId, strMsgData, nMsgType);
                    if(IM::BaseDefine::MsgType_IsValid(nMsgType))
                    {
                        cUnreadInfo.set_latest_msg_id(nMsgId);
                        cUnreadInfo.set_latest_msg_data(strMsgData);
                        cUnreadInfo.set_latest_msg_type(nMsgType);
                        cUnreadInfo.set_latest_msg_from_user_id(cUnreadInfo.session_id());
                        lsUnreadCount.push_back(cUnreadInfo);
                        nTotalCnt += cUnreadInfo.unread_cnt();
                    }
                    else
                    {
                        LOG(ERROR)<<"invalid msgType. userId="<<nUserId<<",peerId="<<cUnreadInfo.session_id()<<",msgType="<<nMsgType;
                    }
                }
            }
            else
            {
                LOG(ERROR)<<"hgetall "<< strKey.c_str()<<" failed!";
            }
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
    }

    uint32_t CMessageModel::getMsgId(uint32_t nRelateId)
    {
        uint32_t nMsgId = 0;
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if(pCacheConn)
        {
            std::string strKey = "msg_id_" + int2string(nRelateId);
            nMsgId = pCacheConn->incrBy(strKey, 1);
            pCacheManager->RelCacheConn(pCacheConn);
        }
        return nMsgId;
    }

/**
 *  <#Description#>
 *
 *  @param nFromId    <#nFromId description#>
 *  @param nToId      <#nToId description#>
 *  @param nMsgId     <#nMsgId description#>
 *  @param strMsgData <#strMsgData description#>
 *  @param nMsgType   <#nMsgType description#>
 *  @param nStatus    0获取未被删除的，1获取所有的，默认获取未被删除的
 */
    void CMessageModel::getLastMsg(uint32_t nFromId, uint32_t nToId, uint32_t& nMsgId, std::string& strMsgData, IM::BaseDefine::MsgType& nMsgType, uint32_t nStatus)
    {
        uint32_t nRelateId = CRelationModel::getInstance()->getRelationId(nFromId, nToId, false);

        if (nRelateId != INVALID_VALUE)
        {

            CDBManager* pDBManager = CDBManager::getInstance();
            CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
            if (pDBConn)
            {
                std::string strTableName = "IMMessage_" + int2string(nRelateId % 8);
                std::string strSql = "select msgId,type,content from " + strTableName + " force index (idx_relateId_status_created) where relateId= " + int2string(nRelateId) + " and status = 0 order by created desc, id desc limit 1";
                CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
                if (pResultSet)
                {
                    while (pResultSet->Next())
                    {
                        nMsgId = pResultSet->GetInt("msgId");

                        nMsgType = IM::BaseDefine::MsgType(pResultSet->GetInt("type"));
                        if (nMsgType == IM::BaseDefine::MSG_TYPE_SINGLE_AUDIO)
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
                    LOG(ERROR)<<"no result set: %s", strSql.c_str();
                }
                pDBManager->RelDBConn(pDBConn);
            }
            else
            {
                LOG(ERROR)<<"no db connection_slave";
            }
        }
        else
        {
            LOG(ERROR)<<"no relation between "<<nFromId<<" and " << nToId;
        }
    }

    void CMessageModel::getUnReadCntAll(uint32_t nUserId, uint32_t &nTotalCnt)
    {
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if (pCacheConn)
        {
            std::map<std::string, std::string> mapUnread;
            std::string strKey = "unread_" + int2string(nUserId);
            bool bRet = pCacheConn->hgetAll(strKey, mapUnread);
            pCacheManager->RelCacheConn(pCacheConn);

            if(bRet)
            {
                for (auto it = mapUnread.begin(); it != mapUnread.end(); it++) {
                    nTotalCnt += std::stoi(it->second.c_str());
                }
            }
            else
            {
                LOG(ERROR)<<"hgetall " <<strKey.c_str() << "failed!";
            }
        }
        else
        {
            LOG(ERROR)<<"no cache connection for unread";
        }
    }

    void CMessageModel::getMsgByMsgId(uint32_t nUserId, uint32_t nPeerId, const std::list<uint32_t> &lsMsgId, std::list<IM::BaseDefine::MsgInfo> &lsMsg)
    {
        if(lsMsgId.empty())

        {
            return ;
        }
        uint32_t nRelateId = CRelationModel::getInstance()->getRelationId(nUserId, nPeerId, false);

        if(nRelateId == INVALID_VALUE)
        {
            LOG(ERROR)<<"invalid relation id between "<<nUserId<<" and "<< nPeerId;
            return;
        }

        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            std::string strTableName = "IMMessage_" + int2string(nRelateId % 8);
            std::string strClause ;
            bool bFirst = true;
            for(auto it= lsMsgId.begin(); it!=lsMsgId.end();++it)
            {
                if (bFirst) {
                    bFirst = false;
                    strClause = int2string(*it);
                }
                else
                {
                    strClause += ("," + int2string(*it));
                }
            }

            std::string strSql = "select * from " + strTableName + " where relateId=" + int2string(nRelateId) + "  and status=0 and msgId in (" + strClause + ") order by created desc, id desc limit 100";
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if (pResultSet)
            {
                while (pResultSet->Next())
                {
                    IM::BaseDefine::MsgInfo msg;
                    msg.set_msg_id(pResultSet->GetInt("msgId"));
                    msg.set_from_session_id(pResultSet->GetInt("fromId"));
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
                        LOG(ERROR)<<"invalid msgType. userId="<<nUserId<<", peerId="<<nPeerId<<", msgType="<<nMsgType<<", msgId="<<msg.msg_id();
                    }
                }
                delete pResultSet;
            }
            else
            {
                LOG(ERROR)<<"no result set for sql:"<< strSql.c_str();
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

    bool CMessageModel::resetMsgId(uint32_t nRelateId)
    {
        bool bRet = false;
        uint32_t nMsgId = 0;
        CacheManager* pCacheManager = CacheManager::getInstance();
        CacheConn* pCacheConn = pCacheManager->GetCacheConn("unread");
        if(pCacheConn)
        {
            std::string strKey = "msg_id_" + int2string(nRelateId);
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