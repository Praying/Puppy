/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include "DebugHelper.hpp"
#include <iostream>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/propertyconfigurator.h>
#include <base/resource_mgr/ResourceMgr.hpp>


#define FLOW_LOG4CXX_INFO(logger, str) \
{\
    try{\
        LOG4CXX_INFO(logger, str);\
    }\
    catch(const log4cxx::helpers::IOException& ioex){\
        std::cerr<<"IOException:"<<ioex.what() <<"\n"<<"INFO="<<str.c_str()<<std::endl;\
    }\
}

#define FLOW_LOG4CXX_DEBUG(logger, str) \
{\
    try{\
        LOG4CXX_DEBUG(logger, str);\
    }\
    catch(const log4cxx::helpers::IOException& ioex){\
        std::cerr<<"IOException:"<<ioex.what() <<"\n"<<"DEBUG="<<str.c_str()<<std::endl;\
    }\
}


#define FLOW_LOG4CXX_WARN(logger, str) \
{\
    try{\
        LOG4CXX_WARN(logger, str);\
    }\
    catch(const log4cxx::helpers::IOException& ioex){\
        std::cerr<<"IOException:"<<ioex.what() <<"\n"<<"WARN="<<str.c_str()<<std::endl;\
    }\
}

#define FLOW_LOG4CXX_ERROR(logger, str) \
{\
    try{\
        LOG4CXX_ERROR(logger, str);\
    }\
    catch(const log4cxx::helpers::IOException& ioex){\
        std::cerr<<"IOException:"<<ioex.what() <<"\n"<<"ERROR="<<str.c_str()<<std::endl;\
    }\
}

#define FLOW_LOG4CXX_FATAL(logger, str) \
{\
    try{\
        LOG4CXX_FATAL(logger, str);\
    }\
    catch(const log4cxx::helpers::IOException& ioex){\
        std::cerr<<"IOException:"<<ioex.what() <<"\n"<<"FATAL="<<str.c_str()<<std::endl;\
    }\
}


#define FLOW_LOG4CXX_LOG(logger, level, str) \
{\
    try{\
        LOG4CXX_LOG(logger, level, str);\
    }\
    catch(const log4cxx::helpers::IOException& ioex){\
        std::cerr<<"IOException:"<<ioex.what() <<"\n"<<"LOG="<<str.c_str()<<std::endl;\
    }\
}


namespace Flow
{

    FLOW_SINGLETON_IMP(DebugHelper);

    DebugHelper::DebugHelper()
    :logger_(log4cxx::Logger::getLogger(""))
    {

    }

    DebugHelper::~DebugHelper()
    {

    }

    void DebugHelper::infoMsg(const std::string &msg)
    {
        FLOW_LOG4CXX_INFO(logger_, msg);
    }

    void DebugHelper::debugMsg(const std::string &msg)
    {
        FLOW_LOG4CXX_DEBUG(logger_, msg);
    }

    void DebugHelper::warnMsg(const std::string &msg)
    {
        FLOW_LOG4CXX_WARN(logger_, msg);
    }

    void DebugHelper::errorMsg(const std::string &msg)
    {
        FLOW_LOG4CXX_ERROR(logger_, msg);
    }

    void DebugHelper::fatalMsg(const std::string &msg)
    {
        FLOW_LOG4CXX_FATAL(logger_, msg);
    }

    void DebugHelper::initialize(Common::ComponentType componentType)
    {
        std::string appName = Common::getComponentNameByType(componentType);
        std::string appConfigName = appName + ".properties";
        auto configPath = ResourceMgr::instance()->matchResource(appConfigName);
        if(configPath)
            log4cxx::PropertyConfigurator::configure((*configPath));
        logger_ = log4cxx::Logger::getRootLogger();
    }

}