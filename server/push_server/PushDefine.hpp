//
// Created by quran on 2018/8/23.
//

#ifndef SERVER_PUSHDEFINE_HPP
#define SERVER_PUSHDEFINE_HPP

#include <cstring>
#include <fmt/format.h>
#include <glog/logging.h>

#include <cstdarg>  // For va_start, etc.
#include <memory>    // For std::unique_ptr

std::string string_format(const std::string fmt_str, ...);
#define PDU_VERSION     1

#define PUSH_TYPE_NORMAL        1
#define PUSH_TYPE_SILENT        2

#define LOG_MODULE_PUSH         "PUSH"


#define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1):__FILE__)

#define PUSH_SERVER_FATAL(fmt, ...) \
{\
    LOG(FATAL)<<string_format(fmt,##__VA_ARGS__);\
}

#define PUSH_SERVER_ERROR(fmt, ...) \
{\
    LOG(ERROR)<<string_format(fmt, ##__VA_ARGS__);\
}

#define PUSH_SERVER_WARN(fmt, ...) \
{\
    LOG(WARNING)<<string_format(fmt, ##__VA_ARGS__);\
}

#define PUSH_SERVER_INFO(fmt, ...) \
{\
    LOG(INFO)<<string_format(fmt, ##__VA_ARGS__);\
}

#define PUSH_SERVER_DEBUG(fmt, ...) \
{\
    LOG(INFO)<<string_format(fmt,##__VA_ARGS__);\
}

#define PUSH_SERVER_TRACE(fmt, ...) \
{\
    LOG(ERROR)<<string_format(fmt,##__VA_ARGS__);\
}


#endif //SERVER_PUSHDEFINE_HPP
