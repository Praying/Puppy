//
// Created by quran on 2018/8/18.
//

#include <db_proxy_server/DBPool.hpp>
#include <base/common/Util.hpp>
#include <glog/logging.h>
#include <base/common/OSType.hpp>
#include "CSessionModel.hpp"
#include "MessageModel.hpp"
#include "CGroupMessageModel.hpp"

namespace Flow{
    CSessionModel* CSessionModel::m_pInstance = NULL;

    CSessionModel* CSessionModel::getInstance()
    {
        if (!m_pInstance) {
            m_pInstance = new CSessionModel();
        }

        return m_pInstance;
    }

    void CSessionModel::getRecentSession(uint32_t nUserId, uint32_t lastTime, std::list<IM::BaseDefine::ContactSessionInfo>& lsContact)
    {
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            std::string strSql = "select * from IMRecentSession where userId = " + Util::int2string(nUserId) + " and status = 0 and updated >" + Util::int2string(lastTime) + " order by updated desc limit 100";

            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if (pResultSet)
            {
                while (pResultSet->Next())
                {
                    IM::BaseDefine::ContactSessionInfo cRelate;
                    uint32_t nPeerId = pResultSet->GetInt("peerId");
                    cRelate.set_session_id(nPeerId);
                    cRelate.set_session_status(::IM::BaseDefine::SessionStatusType(pResultSet->GetInt("status")));

                    IM::BaseDefine::SessionType nSessionType = IM::BaseDefine::SessionType(pResultSet->GetInt("type"));
                    if(IM::BaseDefine::SessionType_IsValid(nSessionType))
                    {
                        cRelate.set_session_type(IM::BaseDefine::SessionType(nSessionType));
                        cRelate.set_updated_time(pResultSet->GetInt("updated"));
                        lsContact.push_back(cRelate);
                    }
                    else
                    {
                        LOG(ERROR)<<"invalid sessionType. userId="<<nUserId<<", peerId="<<nPeerId<<", sessionType="<<nSessionType;
                    }
                }
                delete pResultSet;
            }
            else
            {
                LOG(ERROR)<<"no result set for sql: "<< strSql;
            }
            pDBManager->RelDBConn(pDBConn);
            if(!lsContact.empty())
            {
                fillSessionMsg(nUserId, lsContact);
            }
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_slave";
        }
    }

    uint32_t CSessionModel::getSessionId(uint32_t nUserId, uint32_t nPeerId, uint32_t nType, bool isAll)
    {
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        uint32_t nSessionId = INVALID_VALUE;
        if(pDBConn)
        {
            std::string strSql;
            if (isAll) {
                strSql= "select id from IMRecentSession where userId=" + Util::int2string(nUserId) + " and peerId=" + Util::int2string(nPeerId) + " and type=" + Util::int2string(nType);
            }
            else
            {
                strSql= "select id from IMRecentSession where userId=" + Util::int2string(nUserId) + " and peerId=" + Util::int2string(nPeerId) + " and type=" + Util::int2string(nType) + " and status=0";
            }

            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while (pResultSet->Next()) {
                    nSessionId = pResultSet->GetInt("id");
                }
                delete pResultSet;
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_slave";
        }
        return nSessionId;
    }

    bool CSessionModel::updateSession(uint32_t nSessionId, uint32_t nUpdateTime)
    {
        bool bRet = false;
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            std::string strSql = "update IMRecentSession set `updated`="+Util::int2string(nUpdateTime) + " where id="+Util::int2string(nSessionId);
            bRet = pDBConn->ExecuteUpdate(strSql.c_str());
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_master";
        }
        return bRet;
    }

    bool CSessionModel::removeSession(uint32_t nSessionId)
    {
        bool bRet = false;
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            uint32_t nNow = (uint32_t) time(NULL);
            std::string strSql = "update IMRecentSession set status = 1, updated="+Util::int2string(nNow)+" where id=" + Util::int2string(nSessionId);
            bRet = pDBConn->ExecuteUpdate(strSql.c_str());
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_master";
        }
        return bRet;
    }

    uint32_t CSessionModel::addSession(uint32_t nUserId, uint32_t nPeerId, uint32_t nType)
    {
        uint32_t nSessionId = INVALID_VALUE;

        nSessionId = getSessionId(nUserId, nPeerId, nType, true);
        uint32_t nTimeNow = time(NULL);
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            if(INVALID_VALUE != nSessionId)
            {
                std::string strSql = "update IMRecentSession set status=0, updated=" + Util::int2string(nTimeNow) + " where id=" + Util::int2string(nSessionId);
                bool bRet = pDBConn->ExecuteUpdate(strSql.c_str());
                if(!bRet)
                {
                    nSessionId = INVALID_VALUE;
                }
                LOG(ERROR)<<"has relation ship set status";
            }
            else
            {
                std::string strSql = "insert into IMRecentSession (`userId`,`peerId`,`type`,`status`,`created`,`updated`) values(?,?,?,?,?,?)";
                // 必须在释放连接前delete CPrepareStatement对象，否则有可能多个线程操作mysql对象，会crash
                CPrepareStatement* stmt = new CPrepareStatement();
                if (stmt->Init(pDBConn->GetMysql(), strSql))
                {
                    uint32_t nStatus = 0;
                    uint32_t index = 0;
                    stmt->SetParam(index++, nUserId);
                    stmt->SetParam(index++, nPeerId);
                    stmt->SetParam(index++, nType);
                    stmt->SetParam(index++, nStatus);
                    stmt->SetParam(index++, nTimeNow);
                    stmt->SetParam(index++, nTimeNow);
                    bool bRet = stmt->ExecuteUpdate();
                    if (bRet)
                    {
                        nSessionId = pDBConn->GetInsertId();
                    }
                    else
                    {
                        LOG(ERROR)<<"insert message failed. "<< strSql;
                    }
                }
                delete stmt;
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_master";
        }
        return nSessionId;
    }



    void CSessionModel::fillSessionMsg(uint32_t nUserId, std::list<IM::BaseDefine::ContactSessionInfo>& lsContact)
    {
        for (auto it=lsContact.begin(); it!=lsContact.end();)
        {
            uint32_t nMsgId = 0;
            std::string strMsgData;
            IM::BaseDefine::MsgType nMsgType;
            uint32_t nFromId = 0;
            if( it->session_type() == IM::BaseDefine::SESSION_TYPE_SINGLE)
            {
                nFromId = it->session_id();
                CMessageModel::getInstance()->getLastMsg(it->session_id(), nUserId, nMsgId, strMsgData, nMsgType);
            }
            else
            {
                CGroupMessageModel::getInstance()->getLastMsg(it->session_id(), nMsgId, strMsgData, nMsgType, nFromId);
            }
            if(!IM::BaseDefine::MsgType_IsValid(nMsgType))
            {
                it = lsContact.erase(it);
            }
            else
            {
                it->set_latest_msg_from_user_id(nFromId);
                it->set_latest_msg_id(nMsgId);
                it->set_latest_msg_data(strMsgData);
                it->set_latest_msg_type(nMsgType);
                ++it;
            }
        }
    }
}