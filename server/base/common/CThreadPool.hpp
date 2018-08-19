//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CTHREADPOOL_HPP
#define SERVER_CTHREADPOOL_HPP

#include "CThread.hpp"
#include <list>
#include <base/network/CTask.hpp>

namespace Flow{
    using namespace Flow::Network;
    class CWorkerThread {
    public:
        CWorkerThread();
        ~CWorkerThread();

        static void* StartRoutine(void* arg);

        void Start();
        void Execute();
        void PushTask(CTask* pTask);

        void SetThreadIdx(uint32_t idx) { m_thread_idx = idx; }
    private:

        uint32_t		m_thread_idx;
        uint32_t		m_task_cnt;
        pthread_t		m_thread_id;
        CThreadNotify	m_thread_notify;
        std::list<CTask*>	m_task_list;
    };

    class CThreadPool {
    public:
        CThreadPool();
        virtual ~CThreadPool();

        int Init(uint32_t worker_size);
        void AddTask(CTask* pTask);
        void Destory();
    private:
        uint32_t 		m_worker_size;
        CWorkerThread* 	m_worker_list;
    };
}


#endif //SERVER_CTHREADPOOL_HPP
