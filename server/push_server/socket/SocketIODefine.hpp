//
// Created by quran on 2018/8/21.
//

#ifndef SERVER_SCOKETIODEFINE_HPP
#define SERVER_SCOKETIODEFINE_HPP

#define SOCKET_IO_RESULT_OK                 0x0000  //
#define SOCKET_IO_TCP_RECV_FAILED			0x0001	//tcp recv failed
#define SOCKET_IO_TCP_SEND_FAILED			0x0002  //tcp send failed
#define SOCKET_IO_TCP_CONNECT_FAILED		0x0003	//tcp connect failed

#define SOCKET_IO_UDP_RECV_FAILED			0x0011	//udp recv failed
#define SOCKET_IO_UDP_SEND_FAILED			0x0012  //udp send failed

#define SOCKET_IO_SSL_RECV_FAILED           0x0021  //ssl recv failed
#define SOCKET_IO_SSL_SEND_FAILED           0x0022  //ssl send failed
#define SOCKET_IO_SSL_CONNECT_FAILED        0x0023  //ssl connect failed

#define LOG_MODULE_SOCKET   "SOCKET"

#include <glog/logging.h>
#include <glog/raw_logging.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define SOCKET_IO_FATAL(fmt, ...) \
{\
 RAW_LOG(FATAL,"<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}


#define SOCKET_IO_ERROR(fmt, ...) \
{\
    RAW_LOG(ERROR,"<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define SOCKET_IO_WARN(fmt, ...)  \
{\
    RAW_LOG(WARNING, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define SOCKET_IO_INFO(fmt, ...)  \
{\
    RAW_LOG(INFO, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define SOCKET_IO_DEBUG(fmt, ...)  \
{\
    RAW_LOG(INFO, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define SOCKET_IO_TRACE(fmt, ...)  \
{\
    RAW_LOG(ERROR, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}


#endif //SERVER_SCOKETIODEFINE_HPP
