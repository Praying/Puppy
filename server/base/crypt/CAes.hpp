//
// Created by quran on 2018/8/24.
//

#pragma once

#include <cstdint>
#include <openssl/aes.h>
#include <openssl/md5.h>
#include <string>
namespace Flow {
    std::string base64_decode(const std::string &ascdata);
    std::string base64_encode(const std::string &bindata);

    class CAes
    {
    public:
        CAes(const std::string& strKey);

        int Encrypt(const char* pInData, uint32_t nInLen, char** ppOutData, uint32_t& nOutLen);
        int Decrypt(const char* pInData, uint32_t nInLen, char** ppOutData, uint32_t& nOutLen);
        void Free(char* pData);

    private:
        AES_KEY m_cEncKey;
        AES_KEY m_cDecKey;

    };

    class CMd5
    {
    public:
        static void MD5_Calculate (const char* pContent, unsigned int nLen,char* md5);
    };

}

