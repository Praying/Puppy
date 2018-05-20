/*
 * Created by Praying on 2018/2/8.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <string>
#include <fmt/printf.h>

namespace Flow
{
    /// to diff with stringFormat in Log.hxx, name this function to formatString
    template<typename Format, typename... Args>
    inline std::string formatString(Format &&fmt, Args &&... args)
    {
        return fmt::sprintf(std::forward<Format>(fmt), std::forward<Args>(args)...);
    };

    ///Returns true if the given char pointer is null
    inline bool isFormatEmptyOrNull(const char *fmt)
    {
        return fmt == nullptr;
    }

    ///Returns true if the given std::string is empty
    inline bool isFormatEmptyOrNull(const std::string &fmt)
    {
        return fmt.empty();
    }
}