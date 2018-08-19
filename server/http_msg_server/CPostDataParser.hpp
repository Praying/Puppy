//
// Created by quran on 2018/8/19.
//

#ifndef SERVER_CPOSTDATAPARSER_HPP
#define SERVER_CPOSTDATAPARSER_HPP


#include <cstdint>
#include <list>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>

namespace Flow{
    // jsonp parameter parser
    class CPostDataParser
    {
    public:
        CPostDataParser() {}
        virtual ~CPostDataParser() {}

        bool Parse(const char* content);

        char* GetValue(const char* key);
    private:
        std::map<std::string, std::string> m_post_map;
    };

    char* PackSendResult(uint32_t error_code, const char* error_msg = "");
    char* PackSendCreateGroupResult(uint32_t error_code, const char* error_msg, uint32_t group_id);
    char* PackGetUserIdByNickNameResult(uint32_t result, std::list<IM::BaseDefine::UserInfo> user_list);
}

#endif //SERVER_CPOSTDATAPARSER_HPP
