//
// Created by quran on 2018/8/18.
//

#ifndef SERVER_CONDITION_HPP
#define SERVER_CONDITION_HPP

#include <pthread.h>
#include <cstdint>
#include "CLock.hpp"


namespace Flow{

    class CCondition
    {
    public:
        CCondition(CLock* pLock);
        ~CCondition();
        void wait();
        /*
         * nWaitTime ms
         * if recv a signal then return true;
         * else return false;
         */
        bool waitTime(uint64_t nWaitTime);
        void notify();
        void notifyAll();
    private:
        CLock* m_pLock;
        pthread_cond_t m_cond;
    };
}

#endif //SERVER_CONDITION_HPP
