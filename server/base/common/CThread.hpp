//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CTHREAD_HPP
#define SERVER_CTHREAD_HPP

namespace Flow{
#include <pthread.h>

    class CThread
    {
    public:
        CThread();
        virtual ~CThread();

#ifdef _WIN32
        static DWORD WINAPI StartRoutine(LPVOID lpParameter);
#else
        static void* StartRoutine(void* arg);
#endif

        virtual void StartThread(void);
        virtual void OnThreadRun(void) = 0;
    protected:
#ifdef _WIN32
        DWORD		m_thread_id;
#else
        pthread_t	m_thread_id;
#endif
    };

    class CEventThread : public CThread
    {
    public:
        CEventThread();
        virtual ~CEventThread();

        virtual void OnThreadTick(void) = 0;
        virtual void OnThreadRun(void);
        virtual void StartThread();
        virtual void StopThread();
        bool IsRunning() { return m_bRunning; }
    private:
        bool 		m_bRunning;
    };

    class CThreadNotify
    {
    public:
        CThreadNotify();
        ~CThreadNotify();
        void Lock() { pthread_mutex_lock(&m_mutex); }
        void Unlock() { pthread_mutex_unlock(&m_mutex); }
        void Wait() { pthread_cond_wait(&m_cond, &m_mutex); }
        void Signal() { pthread_cond_signal(&m_cond); }
    private:
        pthread_mutex_t 	m_mutex;
        pthread_mutexattr_t	m_mutexattr;

        pthread_cond_t 		m_cond;
    };

}


#endif //SERVER_CTHREAD_HPP
