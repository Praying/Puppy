/*
 * Created by Praying on 2018/2/28.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include "ResourceMgr.hpp"
#include <algorithm>
#include <boost/algorithm/string.hpp>
#include <optional>
namespace Flow
{
    FLOW_SINGLETON_IMP(ResourceMgr)

    std::optional<std::string> ResourceMgr::matchResource(const std::string &fileName)
    {
        std::optional<std::string> result;
        namespace bfs = boost::filesystem;
        std::string exePath = bfs::initial_path<bfs::path>().string();
        boost::iterator_range<std::string::iterator> itRange = boost::find_last(exePath, "Flow");
        std::string rootPath = std::string(exePath.begin(),itRange.end()+1);
        std::string resPath=rootPath+"Resource";
        const bfs::path resourcePath{resPath};
        const bfs::recursive_directory_iterator end;
        const auto it = std::find_if(bfs::recursive_directory_iterator(resourcePath), end,
        [&fileName](const bfs::directory_entry& e)
        {
            return e.path().filename() == fileName;
        });
        if (it != end)
            result=std::optional<std::string>(it->path().string());
        return result;
    }

    ResourceMgr::ResourceMgr()
    {

    }

}
