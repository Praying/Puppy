//
// Created by quran on 2018/8/20.
//

#include <base/common/PublicDefine.hpp>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include "CUserInfo.hpp"

namespace Flow{
    using namespace IM::BaseDefine;
    CUserInfo::CUserInfo()
    {}

    CUserInfo::~CUserInfo()
    {}

    void CUserInfo::AddClientType(uint32_t client_type)
    {
        std::map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.find(client_type);
        if (it != m_ClientTypeList.end())
        {
            it->second += 1;
        }
        else
        {
            m_ClientTypeList[client_type] = 1;
        }
    }

    void CUserInfo::RemoveClientType(uint32_t client_type)
    {
        std::map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.find(client_type);
        if (it != m_ClientTypeList.end())
        {
            uint32_t count = it->second;
            count -= 1;
            if (count > 0)
            {
                it->second = count;
            }
            else
            {
                m_ClientTypeList.erase(client_type);
            }
        }
    }

    bool CUserInfo::FindRouteConn(CRouteConn *pConn)
    {
        std::set<CRouteConn*>::iterator it = m_RouteConnSet.find(pConn);
        if (it != m_RouteConnSet.end())
        {
            return true;
        }
        else
        {
            return false;
        }
    }

    uint32_t CUserInfo::GetCountByClientType(uint32_t client_type)
    {
        std::map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.find(client_type);
        if (it != m_ClientTypeList.end()) {
            return it->second;
        }
        else
        {
            return 0;
        }
    }

    bool CUserInfo::IsMsgConnNULL()
    {
        if (m_ClientTypeList.size() == 0) {
            return true;
        }
        else
        {
            return false;
        }
    }

    void CUserInfo::ClearClientType()
    {
        m_ClientTypeList.clear();
    }

    bool CUserInfo::IsPCClientLogin()
    {
        bool bRet = false;
        std::map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.begin();
        for (; it != m_ClientTypeList.end(); it++)
        {
            uint32_t client_type = it->first;
            if (CHECK_CLIENT_TYPE_PC(client_type))
            {
                bRet = true;
                break;
            }
        }
        return bRet;
    }

    bool CUserInfo::IsMobileClientLogin()
    {
        bool bRet = false;
        std::map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.begin();
        for (; it != m_ClientTypeList.end(); it++)
        {
            uint32_t client_type = it->first;
            if (CHECK_CLIENT_TYPE_MOBILE(client_type))
            {
                bRet = true;
                break;
            }
        }
        return bRet;
    }

    uint32_t CUserInfo::GetStatus()
    {
        uint32_t status = USER_STATUS_OFFLINE;
        std::map<uint32_t, uint32_t>::iterator it = m_ClientTypeList.begin();
        for (; it != m_ClientTypeList.end(); it++)
        {
            uint32_t client_type = it->first;
            if (CHECK_CLIENT_TYPE_PC(client_type))
            {
                status = USER_STATUS_ONLINE;
                break;
            }
        }
        return status;
    }
}