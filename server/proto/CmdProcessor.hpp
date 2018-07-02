/*
 * Created by Praying on 2018/1/27.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <folly/Singleton.h>
#include <base/network/Packet.hpp>

namespace Flow
{

    class CmdProcessor
    {
        friend class folly::Singleton<CmdProcessor>;

    public:
        static std::shared_ptr<CmdProcessor> instance();

    public:
        void handleLoginAuth(Packet &packet);
    };

}


