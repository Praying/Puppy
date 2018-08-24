//
// Created by quran on 2018/8/24.
//

#include <base/common/OSType.hpp>
#include <base/network/UtilPdu.hpp>
#include <cstring>
#include <cassert>
#include "CAes.hpp"

namespace Flow {
    using namespace Flow::Network;
    CAes::CAes(const std::string& strKey)
    {
        AES_set_encrypt_key((const unsigned char*)strKey.c_str(), 256, &m_cEncKey);
        AES_set_decrypt_key((const unsigned char*)strKey.c_str(), 256, &m_cDecKey);
    }

    int CAes::Encrypt(const char* pInData, uint32_t nInLen, char** ppOutData, uint32_t& nOutLen)
    {
        if(pInData == NULL|| nInLen <=0 )
        {
            return -1;
        }
        uint32_t nRemain = nInLen % 16;
        uint32_t nBlocks = (nInLen + 15) / 16;

        if (nRemain > 12 || nRemain == 0) {
            nBlocks += 1;
        }
        uint32_t nEncryptLen = nBlocks * 16;

        unsigned char* pData = (unsigned char*) calloc(nEncryptLen, 1);
        memcpy(pData, pInData, nInLen);
        unsigned char* pEncData = (unsigned char*) malloc(nEncryptLen);

        CByteStream::WriteUint32((pData + nEncryptLen - 4), nInLen);
        for (uint32_t i = 0; i < nBlocks; i++) {
            AES_encrypt(pData + i * 16, pEncData + i * 16, &m_cEncKey);
        }

        free(pData);
        std::string strEnc((char*)pEncData, nEncryptLen);
        free(pEncData);
        std::string strDec = base64_encode(strEnc);
        nOutLen = (uint32_t)strDec.length();

        char* pTmp = (char*) malloc(nOutLen + 1);
        memcpy(pTmp, strDec.c_str(), nOutLen);
        pTmp[nOutLen] = 0;
        *ppOutData = pTmp;
        return 0;
    }

    int CAes::Decrypt(const char* pInData, uint32_t nInLen, char** ppOutData, uint32_t& nOutLen)
    {
        if(pInData == NULL|| nInLen <=0 )
        {
            return -1;
        }
        std::string strInData(pInData, nInLen);
        std::string strResult = base64_decode(strInData);
        uint32_t nLen = (uint32_t)strResult.length();
        if(nLen == 0)
        {
            return -2;
        }

        const unsigned char* pData = (const unsigned char*) strResult.c_str();

        if (nLen % 16 != 0) {
            return -3;
        }
        // 先申请nLen 个长度，解密完成后的长度应该小于该长度
        char* pTmp = (char*)malloc(nLen + 1);

        uint32_t nBlocks = nLen / 16;
        for (uint32_t i = 0; i < nBlocks; i++) {
            AES_decrypt(pData + i * 16, (unsigned char*)pTmp + i * 16, &m_cDecKey);
        }

        uchar_t* pStart = (uchar_t*)pTmp+nLen-4;
        nOutLen = CByteStream::ReadUint32(pStart);
        //        printf("%u\n", nOutLen);
        if(nOutLen > nLen)
        {
            free(pTmp);
            return -4;
        }
        pTmp[nOutLen] = 0;
        *ppOutData = pTmp;
        return 0;
    }

    void CAes::Free(char* pOutData)
    {
        if(pOutData)
        {
            free(pOutData);
            pOutData = NULL;
        }
    }

    void CMd5::MD5_Calculate(const char *pContent, unsigned int nLen, char *md5)
    {
        uchar_t d[16];
        MD5_CTX ctx;
        MD5_Init(&ctx);
        MD5_Update(&ctx, pContent, nLen);
        MD5_Final(d, &ctx);
        for (int i=0; i<16; ++i) {
            snprintf(md5+(i*2), 32, "%02x", d[i]);
        }
        md5[32] = 0;
        return;
    }

    static const char b64_table[65] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

    static const char reverse_table[128] = {
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64,
            64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 64, 62, 64, 64, 64, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 64, 64, 64, 64, 64, 64,
            64,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, 64, 64, 64, 64, 64,
            64, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, 64, 64, 64, 64, 64
    };

    std::string base64_decode(const std::string &ascdata) {
        std::string retval;
        const std::string::const_iterator last = ascdata.end();
        int bits_collected = 0;
        unsigned int accumulator = 0;

        for (std::string::const_iterator i = ascdata.begin(); i != last; ++i) {
            const int c = *i;
            if (isspace(c) || c == '=') {
                // Skip whitespace and padding. Be liberal in what you accept.
                continue;
            }
            if ((c > 127) || (c < 0) || (reverse_table[c] > 63)) {
                return "";
            }
            accumulator = (accumulator << 6) | reverse_table[c];
            bits_collected += 6;
            if (bits_collected >= 8) {
                bits_collected -= 8;
                retval += (char)((accumulator >> bits_collected) & 0xffu);
            }
        }
        return retval;
    }

    
    std::string base64_encode(const std::string &bindata) {
        using std::numeric_limits;

        if (bindata.size() > (numeric_limits<std::string::size_type>::max() / 4u) * 3u) {
            //throw length_error("Converting too large a std::string to base64.");
            return "";
        }

        const size_t binlen = bindata.size();
        // Use = signs so the end is properly padded.
        std::string retval((((binlen + 2) / 3) * 4), '=');
        size_t outpos = 0;
        int bits_collected = 0;
        unsigned int accumulator = 0;
        const std::string::const_iterator binend = bindata.end();

        for (std::string::const_iterator i = bindata.begin(); i != binend; ++i) {
            accumulator = (accumulator << 8) | (*i & 0xffu);
            bits_collected += 8;
            while (bits_collected >= 6) {
                bits_collected -= 6;
                retval[outpos++] = b64_table[(accumulator >> bits_collected) & 0x3fu];
            }
        }
        if (bits_collected > 0) { // Any trailing bits that are missing.
            assert(bits_collected < 6);
            accumulator <<= 6 - bits_collected;
            retval[outpos++] = b64_table[accumulator & 0x3fu];
        }
        assert(outpos >= (retval.size() - 2));
        assert(outpos <= retval.size());
        return retval;

    }
}