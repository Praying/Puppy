//
// Created by quran on 2018/8/18.
//

#include <db_proxy_server/DBPool.hpp>
#include <base/common/Util.hpp>
#include <glog/logging.h>
#include "CRelationModel.hpp"
#include "MessageModel.hpp"
#include <string>
#include <base/common/OSType.hpp>

namespace Flow{
    CRelationModel* CRelationModel::m_pInstance = NULL;

    CRelationModel::CRelationModel()
    {

    }

    CRelationModel::~CRelationModel()
    {

    }

    CRelationModel* CRelationModel::getInstance()
    {
        if (!m_pInstance) {
            m_pInstance = new CRelationModel();
        }

        return m_pInstance;
    }

/**
 *  获取会话关系ID
 *  对于群组，必须把nUserBId设置为群ID
 *
 *  @param nUserAId  <#nUserAId description#>
 *  @param nUserBId  <#nUserBId description#>
 *  @param bAdd      <#bAdd description#>
 *  @param nStatus 0 获取未被删除会话，1获取所有。
 */
    uint32_t CRelationModel::getRelationId(uint32_t nUserAId, uint32_t nUserBId, bool bAdd)
    {
        uint32_t nRelationId = INVALID_VALUE;
        if (nUserAId == 0 || nUserBId == 0) {
            LOG(ERROR)<<"invalied user id:"<<nUserAId<<"->"<<nUserBId;
            return nRelationId;
        }
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            uint32_t nBigId = nUserAId > nUserBId ? nUserAId : nUserBId;
            uint32_t nSmallId = nUserAId > nUserBId ? nUserBId : nUserAId;
            std::string strSql = "select id from IMRelationShip where smallId=" + Util::int2string(nSmallId) + " and bigId="+ Util::int2string(nBigId) + " and status = 0";

            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if (pResultSet)
            {
                while (pResultSet->Next())
                {
                    nRelationId = pResultSet->GetInt("id");
                }
                delete pResultSet;
            }
            else
            {
                LOG(ERROR)<<"there is no result for sql:"<< strSql.c_str();
            }
            pDBManager->RelDBConn(pDBConn);
            if (nRelationId == INVALID_VALUE && bAdd)
            {
                nRelationId = addRelation(nSmallId, nBigId);
            }
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_slave";
        }
        return nRelationId;
    }

    uint32_t CRelationModel::addRelation(uint32_t nSmallId, uint32_t nBigId)
    {
        uint32_t nRelationId = INVALID_VALUE;
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            uint32_t nTimeNow = (uint32_t)time(NULL);
            std::string strSql = "select id from IMRelationShip where smallId=" + Util::int2string(nSmallId) + " and bigId="+ Util::int2string(nBigId);
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet && pResultSet->Next())
            {
                nRelationId = pResultSet->GetInt("id");
                strSql = "update IMRelationShip set status=0, updated=" + Util::int2string(nTimeNow) + " where id=" + Util::int2string(nRelationId);
                bool bRet = pDBConn->ExecuteUpdate(strSql.c_str());
                if(!bRet)
                {
                    nRelationId = INVALID_VALUE;
                }
                LOG(ERROR)<<"has relation ship set status";
                delete pResultSet;
            }
            else
            {
                strSql = "insert into IMRelationShip (`smallId`,`bigId`,`status`,`created`,`updated`) values(?,?,?,?,?)";
                // 必须在释放连接前delete CPrepareStatement对象，否则有可能多个线程操作mysql对象，会crash
                CPrepareStatement* stmt = new CPrepareStatement();
                if (stmt->Init(pDBConn->GetMysql(), strSql))
                {
                    uint32_t nStatus = 0;
                    uint32_t index = 0;
                    stmt->SetParam(index++, nSmallId);
                    stmt->SetParam(index++, nBigId);
                    stmt->SetParam(index++, nStatus);
                    stmt->SetParam(index++, nTimeNow);
                    stmt->SetParam(index++, nTimeNow);
                    bool bRet = stmt->ExecuteUpdate();
                    if (bRet)
                    {
                        nRelationId = pDBConn->GetInsertId();
                    }
                    else
                    {
                        LOG(ERROR)<<"insert message failed. "<< strSql.c_str();
                    }
                }
                if(nRelationId != INVALID_VALUE)
                {
                    // 初始化msgId
                    if(!CMessageModel::getInstance()->resetMsgId(nRelationId))
                    {
                        LOG(ERROR)<<"reset msgId failed. smallId="<<nSmallId<<", bigId=%u."<<nBigId;

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
        return nRelationId;
    }

    bool CRelationModel::updateRelation(uint32_t nRelationId, uint32_t nUpdateTime)
    {
        bool bRet = false;
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            std::string strSql = "update IMRelationShip set `updated`="+Util::int2string(nUpdateTime) + " where id="+Util::int2string(nRelationId);
            bRet = pDBConn->ExecuteUpdate(strSql.c_str());
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_master";
        }
        return bRet;
    }

    bool CRelationModel::removeRelation(uint32_t nRelationId)
    {
        bool bRet = false;
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_master");
        if (pDBConn)
        {
            uint32_t nNow = (uint32_t) time(NULL);
            std::string strSql = "update IMRelationShip set status = 1, updated="+Util::int2string(nNow)+" where id=" + Util::int2string(nRelationId);
            bRet = pDBConn->ExecuteUpdate(strSql.c_str());
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<"no db connection for teamtalk_master";
        }
        return bRet;
    }
}