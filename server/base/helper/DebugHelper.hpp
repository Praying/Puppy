/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once
#include <folly/Singleton.h>
#include <log4cplus/logger.h>
#include <base/common/Define.hpp>
#include <base/common/Common.hpp>
#include <fmt/format.h>
namespace Flow
{

    class DebugHelper
    {
       FLOW_SINGLETON_DECLARE(DebugHelper);
    protected:
        DebugHelper();
    public:
        ~DebugHelper();

        void initialize(Common::ComponentType componentType);
        void infoMsg(const std::string& msg);
        void debugMsg(const std::string& msg);
        void warnMsg(const std::string& msg);
        void errorMsg(const std::string& msg);
        void fatalMsg(const std::string& msg);
    private:
        log4cplus::Logger logger_;
    };

    #define __FILENAME__ (strrchr(__FILE__, '/') ? (strrchr(__FILE__, '/') + 1) : __FILE__)

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


