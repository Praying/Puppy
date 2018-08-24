//
// Created by quran on 2018/8/23.
//

#ifndef SERVER_CAPNSCLIENT_HPP
#define SERVER_CAPNSCLIENT_HPP


#include <push_server/socket/CEpollIOLoop.hpp>
#include <push_server/timer/BaseTimer.hpp>
#include <memory>
#include <string>
#include "CSSLClientAsync.hpp"
#include "CAPNSClientHandler.hpp"
#include "CAPNSFeedBackHandler.hpp"

namespace Flow::PushServer{

    typedef struct tag_apn_server {
        const char *host;
        int port;
    }ST_APNS, *LPST_APNS;
    static ST_APNS s_apn_servers[4] =
            {
                    {"gateway.sandbox.push.apple.com", 2195},
                    {"feedback.sandbox.push.apple.com", 2196},
                    {"gateway.push.apple.com", 2195},
                    {"feedback.push.apple.com", 2196}
            };


    class CAPNSClient : public std::enable_shared_from_this<CAPNSClient>
    {
    public:
        CAPNSClient(CEpollIOLoop& io);
        virtual ~CAPNSClient();

        static void TimerProc(int nIndex, void* param);

        void SetCertPath(const char* cert_path) { m_strCertPath = cert_path; }
        std::string GetCertPath() { return m_strCertPath; }
        void SetKeyPath(const char* key_path) { m_strKeyPath = key_path; }
        std::string GetKeyPath() { return m_strKeyPath; }
        void SetKeyPassword(const char* key_password) { m_strKeyPassword = key_password; }
        std::string GetKeyPassword() { return m_strKeyPassword; }
        void SetSandBox(BOOL bSandBox) { m_bSandBox = bSandBox; }
        BOOL GetSandBox() { return m_bSandBox; }
        std::string GetGatewayIP() { return m_strGatewayIP; }
        uint32_t GetGatewayPort() { return m_nGatewayPort; }
        std::string GetFeedbackIP() { return m_strFeedbackIP; }
        uint32_t GetFeedbackPort() { return m_nFeedbackPort; }

        BOOL Start();
        BOOL Stop();
        BOOL StopGateWayClient();
        BOOL StopFeedBackClient();

        BOOL ConnectGateway();
        BOOL ConnectFeedback();
        BOOL ReConnectGateway();
        BOOL ReConnectFeedback();

        void CheckConnectGateway();
        void CheckConnectFeedback();

        void StartReConnectGateway();
        void StopReConnectGateway();
        void StartCheckConnectGateway();
        void StopCheckConnectGateway();

        void StartReConnectFeedback();
        void StopReConnectFeedback();
        void StartCheckConnectFeedback();
        void StopCheckConnectFeedback();

        BOOL SendPushMsgToGateway(const char* szBuf, uint32_t nBufLen);
    private:
        BOOL _GetAPNSServerAddress();
    private:
        std::string m_strCertPath;
        std::string m_strKeyPath;
        std::string m_strKeyPassword;
        BOOL   m_bSandBox;
        std::string m_strGatewayIP;
        uint32_t m_nGatewayPort;
        std::string m_strFeedbackIP;
        uint32_t m_nFeedbackPort;

        CSSLClientAsync* m_pGatewayClient;
        CSSLClientAsync* m_pFeedbackClient;
        CEpollIOLoop& m_io;

        CAPNSClientHandler m_gatewayhandler;
        CAPNSFeedBackHandler m_feedbackhandler;

        CTimer m_timer;
    };

    typedef std::shared_ptr<CAPNSClient> apns_client_ptr;
}


#endif //SERVER_CAPNSCLIENT_HPP
