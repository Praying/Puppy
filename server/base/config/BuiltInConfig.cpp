/*
 * Created by Praying on 2018/2/10.
 * Copyright (c) Flow. All rights reserved.
 */

#include <base/config/BuiltInConfig.hpp>
#include <base/config/ConfigManager.hpp>
#include <base/common/GitRevision.hpp>

namespace Flow
{
    namespace BuiltInConfig
    {

        template<typename Fn>
        static std::string getStringWithDefaultValueFromFunction(const std::string &key, Fn getter)
        {
            const std::string value = sConfigManager->getStringDefault(key, "");
            return value.empty() ? getter() : value;
        }

        std::string getCMakeCommand()
        {
            return getStringWithDefaultValueFromFunction("CMakeCommand", GitRevision::getCMakeCommand);
        }

        std::string getBuildDirector()
        {
            return getStringWithDefaultValueFromFunction("BuildDirectory", GitRevision::getBuildDirectory);
        }

        std::string getSourceDirectory()
        {
            return getStringWithDefaultValueFromFunction("SourceDirectory", GitRevision::getSourceDirectory);
        }

        std::string getMySQLExecutable()
        {
            return getStringWithDefaultValueFromFunction("MySQLExecutable", GitRevision::getMySQLExecutable);
        }


    }

}