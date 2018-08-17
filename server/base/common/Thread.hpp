//
// Created by quran on 2018/8/17.
//

#ifndef SERVER_THREAD_HPP
#define SERVER_THREAD_HPP

#include <pthread.h>

namespace Flow{
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
#endif //SERVER_THREAD_HPP
