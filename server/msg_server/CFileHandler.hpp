//
// Created by quran on 2018/8/24.
//

#pragma once

#include <base/network/ImPduBase.hpp>

namespace Flow::MsgServer {
    using namespace Flow::Network;
    class CMsgConn;

    class CFileHandler
    {
    public:
        virtual ~CFileHandler() {}

        static CFileHandler* getInstance();

        void HandleClientFileRequest(CMsgConn* pMsgConn, CImPdu* pPdu);
        void HandleClientFileHasOfflineReq(CMsgConn* pMsgConn, CImPdu* pPdu);
        void HandleClientFileAddOfflineReq(CMsgConn* pMsgConn, CImPdu* pPdu);
        void HandleClientFileDelOfflineReq(CMsgConn* pMsgConn, CImPdu* pPdu);
        void HandleFileHasOfflineRes(CImPdu* pPdu);
        void HandleFileNotify(CImPdu* pPdu);
    private:
        CFileHandler() {}

    private:
        static CFileHandler* s_handler_instance;
    };
}

