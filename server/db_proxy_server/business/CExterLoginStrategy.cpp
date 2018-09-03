//
// Created by quran on 2018/9/4.
//

#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include "CExterLoginStrategy.hpp"

namespace Flow::DB_PROXY {
    const std::string strLoginUrl = "http://xxxx";
    bool CExterLoginStrategy::doLogin(const std::string &strName, const std::string &strPass, IM::BaseDefine::UserInfo& user)
    {
        bool bRet = false;

        return bRet;
    }
}