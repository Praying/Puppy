//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CGROUPMESSAGEMODEL_HPP
#define SERVER_CGROUPMESSAGEMODEL_HPP

#include <cstdint>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>

namespace Flow{
    class CGroupMessageModel {
    public:
        virtual ~CGroupMessageModel();
        static CGroupMessageModel* getInstance();

        bool sendMessage(uint32_t nFromId, uint32_t nGroupId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId, const std::string& strMsgContent);
        bool sendAudioMessage(uint32_t nFromId, uint32_t nGroupId, IM::BaseDefine::MsgType nMsgType, uint32_t nCreateTime, uint32_t nMsgId,const char* pMsgContent, uint32_t nMsgLen);
        void getMessage(uint32_t nUserId, uint32_t nGroupId, uint32_t nMsgId, uint32_t nMsgCnt,
                        std::list<IM::BaseDefine::MsgInfo>& lsMsg);
        bool clearMessageCount(uint32_t nUserId, uint32_t nGroupId);
        uint32_t getMsgId(uint32_t nGroupId);
        void getUnreadMsgCount(uint32_t nUserId, uint32_t &nTotalCnt, std::list<IM::BaseDefine::UnreadInfo>& lsUnreadCount);
        void getLastMsg(uint32_t nGroupId, uint32_t& nMsgId, std::string& strMsgData, IM::BaseDefine::MsgType & nMsgType, uint32_t& nFromId);
        void getUnReadCntAll(uint32_t nUserId, uint32_t &nTotalCnt);
        void getMsgByMsgId(uint32_t nUserId, uint32_t nGroupId, const std::list<uint32_t>& lsMsgId, std::list<IM::BaseDefine::MsgInfo>& lsMsg);
        bool resetMsgId(uint32_t nGroupId);
    private:
        CGroupMessageModel();
        bool incMessageCount(uint32_t nUserId, uint32_t nGroupId);

    private:
        static CGroupMessageModel*	m_pInstance;
    };

}

#endif //SERVER_CGROUPMESSAGEMODEL_HPP
