//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CHTTPCLIENT_HPP
#define SERVER_CHTTPCLIENT_HPP

#include <base/common/PublicDefine.hpp>
#include <curl/curl.h>

namespace Flow::Network{
    class CHttpClient
    {
    public:
        CHttpClient(void);
        ~CHttpClient(void);

    public:
        CURLcode Post(const std::string & strUrl, const std::string & strPost, std::string & strResponse);
        CURLcode Get(const std::string & strUrl, std::string & strResponse);
        std::string UploadByteFile(const std::string &url, void* data, int data_len);
        bool DownloadByteFile(const std::string &url, AudioMsgInfo* pAudioMsg);
    };
}



#endif //SERVER_CHTTPCLIENT_HPP
