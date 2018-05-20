/*
 * Created by Praying on 2018/2/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <base/network/MessageBuffer.hpp>

namespace Flow
{
    namespace Util
    {
        bool stringToBool(std::string const &str);
        boost::program_options::variables_map getConsoleArguments(int argc, char **argv);
        int encodeMsg(const uint16_t type,const uint16_t msgID, const char* strData, const uint32_t dataLen, MessageBuffer& buffer);
    }
}