//
// Created by quran on 2018/8/21.
//

#ifndef SERVER_CEPOLLIOLOOP_HPP
#define SERVER_CEPOLLIOLOOP_HPP

#include "SocketIO.hpp"
#include "CBaseIOStream.hpp"
#include "CIOLoop.hpp"
#if defined(__linux__)

#include <sys/epoll.h>

#endif
#define EPOLL_SIZE  256

namespace Flow::PushServer {

    using namespace std;


    class SOCKET_IO_DECLARE_CLASS CEpollIOLoop : public CIOLoop {
    public:

        CEpollIOLoop(void);

        virtual ~CEpollIOLoop(void);

#if defined(__linux__)

        virtual void Start(int32_t nEpollSize = EPOLL_SIZE);

        virtual void Stop();

        virtual void Run();

        virtual void Add_Handler(CBaseIOStream *piostream);

        virtual void Remove_Handler(CBaseIOStream *piostream);

        virtual void Add_WriteEvent(CBaseIOStream *piostream);

        virtual void Remove_WriteEvent(CBaseIOStream *piostream);

    private:

        void _SetEpollSize(int32_t nSize) { m_epollszie = nSize; }

        int _GetEpollSize() { return m_epollszie; }

#endif

    private:
        int32_t m_eid;
        int32_t m_epollszie;

    };
}

#endif //SERVER_CEPOLLIOLOOP_HPP
