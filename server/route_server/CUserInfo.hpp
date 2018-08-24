//
// Created by quran on 2018/8/20.
//

#ifndef SERVER_CUSERINFO_HPP
#define SERVER_CUSERINFO_HPP

#include <set>
#include <map>

namespace Flow{
    class CRouteConn;
    
    class CUserInfo
    {
    public:
        CUserInfo();
        ~CUserInfo();

        uint32_t GetStatus();

        void AddRouteConn(CRouteConn* pConn) { m_RouteConnSet.insert(pConn); }
        void RemoveRouteConn(CRouteConn* pConn) { m_RouteConnSet.erase(pConn); }
        void ClearRouteConn() { m_RouteConnSet.clear(); }
        std::set<CRouteConn* >* GetRouteConn() { return &m_RouteConnSet; }
        bool FindRouteConn(CRouteConn* pConn);

        uint32_t GetRouteConnCount() { return m_RouteConnSet.size(); }
        void AddClientType(uint32_t client_type);
        void RemoveClientType(uint32_t client_type);
        uint32_t GetCountByClientType(uint32_t client_type);
        bool IsMsgConnNULL();
        void ClearClientType();

        bool IsPCClientLogin();
        bool IsMobileClientLogin();
    private:
        std::set<CRouteConn* > m_RouteConnSet;
        std::map<uint32_t /* client_type */, uint32_t /* count */> m_ClientTypeList;
    };
}

#endif //SERVER_CUSERINFO_HPP
