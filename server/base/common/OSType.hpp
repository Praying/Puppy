//
// Created by quran on 2018/8/17.
//

#ifndef SERVER_OSTYPE_HPP
#define SERVER_OSTYPE_HPP

#ifdef _WIN32
#include <WinSock2.h>
    #include <WinBase.h>
    #include <Windows.h>
    #include <direct.h>
#else
#ifdef __APPLE__
#include <sys/event.h>
        #include <sys/time.h>
        #include <sys/syscall.h>	// syscall(SYS_gettid)
#else
#include <sys/epoll.h>
#endif
#include <pthread.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <errno.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdint.h>		// define int8_t ...
#include <signal.h>
#include <unistd.h>
#define closesocket close
#define ioctlsocket ioctl
#endif

#ifdef _WIN32
typedef char			int8_t;
    typedef short			int16_t;
    typedef int				int32_t;
    typedef	long long		int64_t;
    typedef unsigned char	uint8_t;
    typedef unsigned short	uint16_t;
    typedef unsigned int	uint32_t;
    typedef	unsigned long long	uint64_t;
    typedef int				socklen_t;
#else
typedef int SOCKET;
typedef int BOOL;
#ifndef  __APPLE__
const int TRUE = 1;
const int FALSE = 0;
#endif
const int SOCKET_ERROR = -1;
const int INVALID_SOCKET = -1;
#endif

enum {
    NETLIB_OK		= 0,
    NETLIB_ERROR	= -1
};

#define NETLIB_INVALID_HANDLE	-1

#define NOTUSED_ARG(v) ((void)v)

enum {
    NETLIB_MSG_CONNECT = 1,
    NETLIB_MSG_CONFIRM,
    NETLIB_MSG_READ,
    NETLIB_MSG_WRITE,
    NETLIB_MSG_CLOSE,
    NETLIB_MSG_TIMER,
    NETLIB_MSG_LOOP
};

typedef unsigned char uchar_t;
typedef int net_handle_t;
typedef int conn_handle_t;

typedef void (*callback_t)(void *callback_data, uint8_t msg, uint32_t handle, void *pParam);


#define     GROUP_TOTAL_MSG_COUNTER_REDIS_KEY_SUFFIX    "_im_group_msg"
#define     GROUP_USER_MSG_COUNTER_REDIS_KEY_SUFFIX     "_im_user_group"
#define     GROUP_COUNTER_SUBKEY_COUNTER_FIELD          "count"

const uint32_t INVALID_UINT32  = (uint32_t) -1;
const uint32_t INVALID_VALUE = 0;

#define VERSION "0.0.1"

#endif //SERVER_OSTYPE_HPP
