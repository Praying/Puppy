//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CSESSIONMODEL_HPP
#define SERVER_CSESSIONMODEL_HPP

#include <list>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>

namespace Flow{
    class CSessionModel
    {
    public:
        static CSessionModel* getInstance();
        ~CSessionModel() {}

        void getRecentSession(uint32_t userId, uint32_t lastTime, std::list<IM::BaseDefine::ContactSessionInfo>& lsContact);
        uint32_t getSessionId(uint32_t nUserId, uint32_t nPeerId, uint32_t nType, bool isAll);
        bool updateSession(uint32_t nSessionId, uint32_t nUpdateTime);
        bool removeSession(uint32_t nSessionId);
        uint32_t addSession(uint32_t nUserId, uint32_t nPeerId, uint32_t nType);

    private:
        CSessionModel() {};
        void fillSessionMsg(uint32_t nUserId, std::list<IM::BaseDefine::ContactSessionInfo>& lsContact);
    private:
        static CSessionModel* m_pInstance;
    };
}

#endif //SERVER_CSESSIONMODEL_HPP
