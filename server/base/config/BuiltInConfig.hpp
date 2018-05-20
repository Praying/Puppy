/*
 * Created by Praying on 2018/2/10.
 * Copyright (c) Flow. All rights reserved.
 */


#pragma once

#include <string>

namespace Flow
{

    namespace BuiltInConfig
    {
        /// Returns the CMake Command when any is specified in the config
        /// returns the built-in path otherwise
        std::string getCMakeCommand();

        /// Returns the build directory path when any is specified in the config
        /// returns the built-in one otherwise
        std::string getBuildDirector();

        /// Returns the source directory path when any is specified in the config
        /// returns the built-in one otherwise
        std::string getSourceDirectory();

        /// Returns the path to the mysql executable (`mysql`) when any is specified
        /// in the config, returns the built-in one otherwise
        std::string getMySQLExecutable();
    };

}


