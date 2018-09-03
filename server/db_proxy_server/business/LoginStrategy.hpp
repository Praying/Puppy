//
// Created by quran on 2018/9/3.
//

#ifndef SERVER_LOGINSTRATEGY_HPP
#define SERVER_LOGINSTRATEGY_HPP


namespace Flow::DB_PROXY{
    class CLoginStrategy
    {
    public:
        virtual bool doLogin(const std::string& strName, const std::string& strPass, IM::BaseDefine::UserInfo& user) = 0;
    };
}

#endif //SERVER_LOGINSTRATEGY_HPP
