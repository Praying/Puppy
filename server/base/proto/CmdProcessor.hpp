/*
 * Created by Praying on 2018/1/27.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once


#include <base/network/Packet.hpp>

namespace Flow
{

    class CmdProcessor
    {



    public:
        void handleLoginAuth(Packet &packet);
    };

}


