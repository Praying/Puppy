/*
 * Created by Praying on 2018/1/27.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include <proto/CmdProcessor.hpp>

namespace Flow
{
    static folly::Singleton<CmdProcessor> instance_;

    std::shared_ptr<Flow::CmdProcessor> CmdProcessor::instance()
    {
        return instance_.try_get();
    }

    void CmdProcessor::handleLoginAuth(Packet &packet)
    {

    }
}
