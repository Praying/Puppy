/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include "DebugHelper.hpp"
#include <iostream>
#include <base/resource_mgr/ResourceMgr.hpp>
#include <iostream>
#include <spdlog/async.h>

namespace Flow
{

    FLOW_SINGLETON_IMP(DebugHelper);

    DebugHelper::DebugHelper()
    {

    }

    DebugHelper::~DebugHelper()
    {

    }

    void DebugHelper::infoMsg(const std::string &msg)
    {
        logger_->info(msg);
    }

    void DebugHelper::debugMsg(const std::string &msg)
    {
        logger_->debug(msg);
    }

    void DebugHelper::warnMsg(const std::string &msg)
    {
        logger_->warn(msg);
    }

    void DebugHelper::errorMsg(const std::string &msg)
    {
        logger_->error(msg);
    }

    void DebugHelper::fatalMsg(const std::string &msg)
    {
        logger_->critical(msg);
    }

    bool DebugHelper::initialize(Common::ComponentType componentType)
    {
        try {
            std::string appName = Common::getComponentNameByType(componentType);
            logger_ = spdlog::daily_logger_mt<spdlog::create_async>(appName, "logs/daily.txt", 2, 30) ;
            logger_->flush_on(spdlog::level::err);
            spdlog::set_pattern("[%^+++%$] [%H:%M:%S %z] [thread %t] %v");
            spdlog::set_level(spdlog::level::debug);
            logger_->error(2);

        }catch (const spdlog::spdlog_ex& ex){
            std::cout<<"Log init failed: " << ex.what() << std::endl;
            return false;
        }
        return true;
    }

    std::shared_ptr<spdlog::logger> DebugHelper::getLogger() {
        return logger_;
    }

}