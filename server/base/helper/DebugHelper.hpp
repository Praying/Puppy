/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once
#include <folly/Singleton.h>
#include <base/common/Define.hpp>
#include <base/common/Common.hpp>
#include "spdlog/sinks/daily_file_sink.h"
#include "spdlog/sinks/rotating_file_sink.h"
#include "spdlog/sinks/simple_file_sink.h"
#include "spdlog/sinks/stdout_color_sinks.h"

namespace Flow
{

    class DebugHelper
    {
       FLOW_SINGLETON_DECLARE(DebugHelper);
    protected:
        DebugHelper();
    public:
        ~DebugHelper();

        bool initialize(Common::ComponentType componentType);
        std::shared_ptr<spdlog::logger> getLogger();
        void infoMsg(const std::string& msg);
        void debugMsg(const std::string& msg);
        void warnMsg(const std::string& msg);
        void errorMsg(const std::string& msg);
        void fatalMsg(const std::string& msg);
    private:
        std::shared_ptr<spdlog::logger> logger_;
    };

    #define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)



/*#ifdef _WINDOWS_
    #define FLOW_INFO(str, ...) DebugHelper::instance()->infoMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__,##__VA_ARGS__))
    #define FLOW_DEBUG(str, ...) DebugHelper::instance()->debugMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__))
    #define FLOW_WARN(str, ...) DebugHelper::instance()->warnMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__))
    #define FLOW_ERROR(str, ...) DebugHelper::instance()->errorMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__))
#else
    #define FLOW_INFO(str, args...) DebugHelper::instance()->infoMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))
    #define FLOW_DEBUG(str, args...) DebugHelper::instance()->debugMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))
    #define FLOW_WARN(str, args...) DebugHelper::instance()->warnMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))
    #define FLOW_ERROR(str, args...) DebugHelper::instance()->errorMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))

#endif*/

#ifdef _WINDOWS_
    #define FLOW_INFO(str, ...) DebugHelper::instance()->infoMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__,##__VA_ARGS__))
    #define FLOW_DEBUG(str, ...) DebugHelper::instance()->debugMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__))
    #define FLOW_WARN(str, ...) DebugHelper::instance()->warnMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__))
    #define FLOW_ERROR(str, ...) DebugHelper::instance()->errorMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##__VA_ARGS__))
#else
    #define FLOW_INFO(str, args...) DebugHelper::instance()->infoMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))
    #define FLOW_DEBUG(str, args...) DebugHelper::instance()->debugMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))
    #define FLOW_WARN(str, args...) DebugHelper::instance()->warnMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))
    #define FLOW_ERROR(str, args...) DebugHelper::instance()->errorMsg(fmt::format(std::string("[{}|{}|{}] ")+str, __FILENAME__, __LINE__, __FUNCTION__, ##args))

#endif

}


