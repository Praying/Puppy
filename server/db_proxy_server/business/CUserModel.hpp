//
// Created by quran on 2018/8/19.
//

#ifndef SERVER_CUSERMODEL_HPP
#define SERVER_CUSERMODEL_HPP

#include <cstdint>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <base/common/PublicDefine.hpp>
#include <list>

namespace Flow{
    class CUserModel
    {
    public:
        static CUserModel* getInstance();
        ~CUserModel();
        void getChangedId(uint32_t& nLastTime, std::list<uint32_t>& lsIds);
        void getUsers(std::list<uint32_t> lsIds, std::list<IM::BaseDefine::UserInfo>& lsUsers);
        bool getUser(uint32_t nUserId, DBUserInfo_t& cUser);

        bool updateUser(DBUserInfo_t& cUser);
        bool insertUser(DBUserInfo_t& cUser);
//    void getUserByNick(const std::list<std::string>& lsNicks, std::list<IM::BaseDefine::UserInfo>& lsUsers);
        void clearUserCounter(uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::SessionType nSessionType);
        void setCallReport(uint32_t nUserId, uint32_t nPeerId, IM::BaseDefine::ClientType nClientType);

        bool updateUserSignInfo(uint32_t user_id, const std::string& sign_info);
        bool getUserSingInfo(uint32_t user_id, std::string* sign_info);
        bool updatePushShield(uint32_t user_id, uint32_t shield_status);
        bool getPushShield(uint32_t user_id, uint32_t* shield_status);

    private:
        CUserModel();
    private:
        static CUserModel* m_pInstance;
    };
}

#endif //SERVER_CUSERMODEL_HPP
