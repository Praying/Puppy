//
// Created by quran on 2018/9/3.
//

#include <cstdint>
#include <list>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <base/common/Util.hpp>
#include <db_proxy_server/DBPool.hpp>
#include <glog/logging.h>
#include "CDepartModel.hpp"

namespace Flow {
    CDepartModel* CDepartModel::m_pInstance = NULL;

    CDepartModel* CDepartModel::getInstance()
    {
        if(NULL == m_pInstance)
        {
            m_pInstance = new CDepartModel();
        }
        return m_pInstance;
    }

    void CDepartModel::getChgedDeptId(uint32_t& nLastTime, std::list<uint32_t>& lsChangedIds)
    {
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            std::string strSql = "select id, updated from IMDepart where updated > " + Util::int2string(nLastTime);
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while (pResultSet->Next()) {
                    uint32_t id = pResultSet->GetInt("id");
                    uint32_t nUpdated = pResultSet->GetInt("updated");
                    if(nLastTime < nUpdated)
                    {
                        nLastTime = nUpdated;
                    }
                    lsChangedIds.push_back(id);
                }
                delete  pResultSet;
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(ERROR)<<("no db connection for teamtalk_slave.");
        }
    }

    void CDepartModel::getDepts(std::list<uint32_t>& lsDeptIds, std::list<IM::BaseDefine::DepartInfo>& lsDepts)
    {
        if(lsDeptIds.empty())
        {
            LOG(ERROR)<<"list is empty";
            return;
        }
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            std::string strClause;
            bool bFirst = true;
            for (auto it=lsDeptIds.begin(); it!=lsDeptIds.end(); ++it) {
                if(bFirst)
                {
                    bFirst = false;
                    strClause += Util::int2string(*it);
                }
                else
                {
                    strClause += ("," + Util::int2string(*it));
                }
            }
            std::string strSql = "select * from IMDepart where id in ( " + strClause + " )";
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while (pResultSet->Next()) {
                    IM::BaseDefine::DepartInfo cDept;
                    uint32_t nId = pResultSet->GetInt("id");
                    uint32_t nParentId = pResultSet->GetInt("parentId");
                    std::string strDeptName = pResultSet->GetString("departName");
                    uint32_t nStatus = pResultSet->GetInt("status");
                    uint32_t nPriority = pResultSet->GetInt("priority");
                    if(IM::BaseDefine::DepartmentStatusType_IsValid(nStatus))
                    {
                        cDept.set_dept_id(nId);
                        cDept.set_parent_dept_id(nParentId);
                        cDept.set_dept_name(strDeptName);
                        cDept.set_dept_status(IM::BaseDefine::DepartmentStatusType(nStatus));
                        cDept.set_priority(nPriority);
                        lsDepts.push_back(cDept);
                    }
                }
                delete  pResultSet;
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(INFO)<<"no db connection for teamtalk_slave";
        }
    }

    void CDepartModel::getDept(uint32_t nDeptId, IM::BaseDefine::DepartInfo& cDept)
    {
        CDBManager* pDBManager = CDBManager::getInstance();
        CDBConn* pDBConn = pDBManager->GetDBConn("teamtalk_slave");
        if (pDBConn)
        {
            std::string strSql = "select * from IMDepart where id = " + Util::int2string(nDeptId);
            CResultSet* pResultSet = pDBConn->ExecuteQuery(strSql.c_str());
            if(pResultSet)
            {
                while (pResultSet->Next()) {
                    uint32_t nId = pResultSet->GetInt("id");
                    uint32_t nParentId = pResultSet->GetInt("parentId");
                    std::string strDeptName = pResultSet->GetString("departName");
                    uint32_t nStatus = pResultSet->GetInt("status");
                    uint32_t nPriority = pResultSet->GetInt("priority");
                    if(IM::BaseDefine::DepartmentStatusType_IsValid(nStatus))
                    {
                        cDept.set_dept_id(nId);
                        cDept.set_parent_dept_id(nParentId);
                        cDept.set_dept_name(strDeptName);
                        cDept.set_dept_status(IM::BaseDefine::DepartmentStatusType(nStatus));
                        cDept.set_priority(nPriority);
                    }
                }
                delete  pResultSet;
            }
            pDBManager->RelDBConn(pDBConn);
        }
        else
        {
            LOG(INFO)<<("no db connection for teamtalk_slave");
        }
    }
}