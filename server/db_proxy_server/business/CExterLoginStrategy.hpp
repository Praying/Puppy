//
// Created by quran on 2018/9/4.
//

#pragma once

#include "LoginStrategy.hpp"

namespace Flow::DB_PROXY {

    class CExterLoginStrategy:public CLoginStrategy
    {
    public:
        virtual bool doLogin(const std::string& strName, const std::string& strPass, IM::BaseDefine::UserInfo& user);
    };
}

