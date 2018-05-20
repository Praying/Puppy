/*
 * Created by Praying on 2018/2/8.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include <base/common/Tokenizer.hpp>
#include <cstring>

namespace Flow
{

    Tokenizer::Tokenizer(const std::string &src, const char sep, uint32_t vectorReserve, bool keepEmptyStrings)
    {
        str_ = new char[src.length() + 1];
        memcpy(str_, src.c_str(), src.length() + 1);

        if (vectorReserve)
            storage_.reserve(vectorReserve);

        char *oldPos = str_;
        char *newPos = str_;
        for (;;)
        {
            if (*newPos == sep)
            {
                if (keepEmptyStrings || oldPos != newPos)
                {
                    storage_.push_back(oldPos);
                }
                oldPos = newPos + 1;
                *newPos = '\0';
            }
            else if (*newPos == '\0')
            {
                if (oldPos != newPos)
                    storage_.push_back(oldPos);
                break;
            }

            ++newPos;
        }
    }
}
