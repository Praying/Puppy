//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CAUDIOMODEL_HPP
#define SERVER_CAUDIOMODEL_HPP


#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>
#include <list>

namespace Flow{
    class CAudioModel {
    public:
        virtual ~CAudioModel();

        static CAudioModel* getInstance();
        void setUrl(std::string& strFileUrl);

        bool readAudios(std::list<IM::BaseDefine::MsgInfo>& lsMsg);

        int saveAudioInfo(uint32_t nFromId, uint32_t nToId, uint32_t nCreateTime, const char* pAudioData, uint32_t nAudioLen);

    private:
        CAudioModel();
//    void GetAudiosInfo(uint32_t nAudioId, IM::BaseDefine::MsgInfo& msg);
        bool readAudioContent(uint32_t nCostTime, uint32_t nSize, const std::string& strPath, IM::BaseDefine::MsgInfo& msg);

    private:
        static CAudioModel*	m_pInstance;
        std::string m_strFileSite;
    };
}



#endif //SERVER_CAUDIOMODEL_HPP
