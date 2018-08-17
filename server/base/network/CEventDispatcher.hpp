//
// Created by quran on 2018/8/17.
//

#ifndef SERVER_CEVENTDISPATCHER_HPP
#define SERVER_CEVENTDISPATCHER_HPP


#include <base/common/OSType.hpp>
#include <list>
#include <cstdint>
#include <base/common/CLock.hpp>

namespace Flow::Network {
    enum {
        SOCKET_READ = 0x1,
        SOCKET_WRITE = 0x2,
        SOCKET_EXCEP = 0x4,
        SOCKET_ALL = 0x7
    };

    class CEventDispatch {
    public:
        virtual ~CEventDispatch();

        void AddEvent(SOCKET fd, uint8_t socket_event);

        void RemoveEvent(SOCKET fd, uint8_t socket_event);

        void AddTimer(callback_t callback, void *user_data, uint64_t interval);

        void RemoveTimer(callback_t callback, void *user_data);

        void AddLoop(callback_t callback, void *user_data);

        void StartDispatch(uint32_t wait_timeout = 100);

        void StopDispatch();

        bool isRunning() { return running; }

        static CEventDispatch *Instance();

    protected:
        CEventDispatch();

    private:
        void _CheckTimer();

        void _CheckLoop();

        typedef struct {
            callback_t callback;
            void *user_data;
            uint64_t interval;
            uint64_t next_tick;
        } TimerItem;

    private:
#ifdef _WIN32
        fd_set	m_read_set;
    fd_set	m_write_set;
    fd_set	m_excep_set;
#elif __APPLE__
        int 	m_kqfd;
#else
        int m_epfd;
#endif
        CLock m_lock;
        std::list<TimerItem *> m_timer_list;
        std::list<TimerItem *> m_loop_list;

        static CEventDispatch *m_pEventDispatch;

        bool running;
    };

}


#endif //SERVER_CEVENTDISPATCHER_HPP
