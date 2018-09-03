//
// Created by quran on 2018/9/4.
//

#pragma once

#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include "LoginStrategy.hpp"

namespace Flow::DB_PROXY{

    class CInterLoginStrategy :public CLoginStrategy
    {
    public:
        virtual bool doLogin(const std::string& strName, const std::string& strPass, IM::BaseDefine::UserInfo& user);
    };


}

