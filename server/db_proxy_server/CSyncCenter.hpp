//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CSYNCCENTER_HPP
#define SERVER_CSYNCCENTER_HPP


#include <cstdint>
#include <base/common/CLock.hpp>
#include <pthread.h>
#include <string>
#include <base/common/PublicDefine.hpp>
#include <base/common/Condition.hpp>

namespace Flow{
    class CSyncCenter
    {
    public:
        static CSyncCenter* getInstance();

        uint32_t getLastUpdate() {
            CAutoLock auto_lock(&last_update_lock_);
            return m_nLastUpdate;
        }
        uint32_t getLastUpdateGroup() {
            CAutoLock auto_lock(&last_update_lock_);
            return m_nLastUpdateGroup;
        }
        std::string getDeptName(uint32_t nDeptId);
        void startSync();
        void stopSync();
        void init();
        void updateTotalUpdate(uint32_t nUpdated);

    private:
        void updateLastUpdateGroup(uint32_t nUpdated);

        CSyncCenter();
        ~CSyncCenter();
        static void* doSyncGroupChat(void* arg);

    private:
        void getDept(uint32_t nDeptId, DBDeptInfo_t** pDept);
        DBDeptMap_t* m_pDeptInfo;

        static CSyncCenter* m_pInstance;
        uint32_t    m_nLastUpdateGroup;
        uint32_t    m_nLastUpdate;

        CCondition* m_pCondGroupChat;
        CLock*      m_pLockGroupChat;
        static bool        m_bSyncGroupChatRuning;
        bool m_bSyncGroupChatWaitting;
#ifdef _WIN32
        DWORD		m_nGroupChatThreadId;
#else
        pthread_t	m_nGroupChatThreadId;
#endif
        CLock last_update_lock_;


    };
}

#endif //SERVER_CSYNCCENTER_HPP
