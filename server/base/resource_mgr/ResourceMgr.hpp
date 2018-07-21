/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once


#include <base/common/Define.hpp>
#include <optional>
#include <boost/filesystem.hpp>

namespace Flow
{

    class ResourceMgr
    {
        FLOW_SINGLETON_DECLARE(ResourceMgr);
    protected:
        ResourceMgr();
    public:
        std::optional<std::string> matchResource(const std::string& name);
    };

}


