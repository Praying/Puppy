//
// Created by quran on 2018/8/19.
//

#ifndef SERVER_CFILEMODEL_HPP
#define SERVER_CFILEMODEL_HPP


#include <list>

namespace Flow{
    class CFileModel
    {
    public:
        virtual ~CFileModel();
        static CFileModel* getInstance();

        void getOfflineFile(uint32_t userId, std::list<IM::BaseDefine::OfflineFileInfo>& lsOffline);
        void addOfflineFile(uint32_t fromId, uint32_t toId, std::string& taskId, std::string& fileName, uint32_t fileSize);
        void delOfflineFile(uint32_t fromId, uint32_t toId, std::string& taskId);

    private:
        CFileModel();

    private:
        static CFileModel* m_pInstance;
    };
}

#endif //SERVER_CFILEMODEL_HPP
