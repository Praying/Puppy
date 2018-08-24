//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CPUSHAPP_HPP
#define SERVER_CPUSHAPP_HPP

#include <push_server/type/BaseType.hpp>
#include <push_server/socket/CEpollIOLoop.hpp>

namespace Flow::PushServer {
    class CPushApp {
    public:
        CPushApp();

        virtual ~CPushApp();

        static CPushApp *GetInstance();

        BOOL Init();

        BOOL UnInit();

        BOOL Start();

        BOOL Stop();

        CEpollIOLoop &GetIOLoop() { return m_io; }

    private:
        BOOL m_bInit;
        CEpollIOLoop m_io;
    };
}

#endif //SERVER_CPUSHAPP_HPP
