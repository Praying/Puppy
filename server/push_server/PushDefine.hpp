//
// Created by quran on 2018/8/23.
//

#ifndef SERVER_PUSHDEFINE_HPP
#define SERVER_PUSHDEFINE_HPP

#include <cstring>
#define PDU_VERSION     1

#define PUSH_TYPE_NORMAL        1
#define PUSH_TYPE_SILENT        2

#define LOG_MODULE_PUSH         "PUSH"

#include <glog/logging.h>
#include <glog/raw_logging.h>

#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define PUSH_SERVER_FATAL(fmt, ...) \
{\
    RAW_LOG(FATAL, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define PUSH_SERVER_ERROR(fmt, ...) \
{\
    RAW_LOG(ERROR, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define PUSH_SERVER_WARN(fmt, ...) \
{\
    RAW_LOG(WARNING, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define PUSH_SERVER_INFO(fmt, ...) \
{\
    RAW_LOG(INFO, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define PUSH_SERVER_DEBUG(fmt, ...) \
{\
    RAW_LOG(INFO,"<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}

#define PUSH_SERVER_TRACE(fmt, ...) \
{\
    RAW_LOG(ERROR, "<%s>|<%d>|<%s>," fmt, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__);\
}


#endif //SERVER_PUSHDEFINE_HPP
