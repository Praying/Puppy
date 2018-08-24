//
// Created by quran on 2018/8/23.
//

#ifndef SERVER_CSSLCLIENTASYNC_HPP
#define SERVER_CSSLCLIENTASYNC_HPP

#include <push_server/socket/CIOLoop.hpp>
#include "CTCPClientAsync.hpp"
#include <openssl/ssl.h>
#include <openssl/bio.h>
#include <string>
namespace Flow::PushServer{
    class CSSLClientAsync : public CTCPClientAsync
    {
    public:
        CSSLClientAsync(CIOLoop* pIO);
        virtual ~CSSLClientAsync();

        SSL_CTX* GetSSLCTX() const { return m_ctx; }
        SSL* GetSSL() const { return m_ssl; }

        void SetSSLConnectStatus(BOOL bStatus) { m_bSSLConnectStatus = bStatus; }
        BOOL GetSSLConnectStatus() { return m_bSSLConnectStatus; }
        std::string GetCertFile() { return m_strCertFile; }
        std::string GetKeyFile() { return m_strKeyFile; }
        std::string GetKeyPassword() { return m_strKeyPassword; }

        BOOL InitSSL(const char* cert_file = NULL, const char* key_file = NULL, const char* key_password = NULL);
        void UnInitSSL();
        virtual void OnConnect(BOOL bConnected);
        virtual void OnRecv();
        int32_t SSLConnect();
        virtual int32_t ReConnectAsync();

        virtual int32_t SendMsgAsync(const char* szBuf, int32_t nBufSize);
        virtual int32_t SendBufferAsync();

        virtual void Close();
        virtual void ShutDown();

        /*uint32_t nsockid*/
        sigslot::signal1<uint32_t> DoSSLConnect;

    protected:
        virtual void _Close();

    protected:
        BOOL m_bSSLConnectStatus;

        std::string m_strCertFile;
        std::string m_strKeyFile;
        std::string m_strKeyPassword;

        SSL_CTX* m_ctx;
        SSL* m_ssl;
    };

}

#endif //SERVER_CSSLCLIENTASYNC_HPP
