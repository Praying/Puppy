/*
 * Created by Praying on 2018/3/24.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>
#include <asio.hpp>
namespace Flow
{

    enum class ServerType{
        ServerType_None,
        ServerType_Route,
        ServerType_Messager,
    };

    class ServerIndex
    {
        std::string serverName_="";
        ServerType  serverType_=ServerType::ServerType_None;

        bool operator=(const ServerIndex& rhs){
            return this->serverName_==rhs.serverName_&&
                   this->serverType_==rhs.serverType_;
        }
    };

    class ServerInfo{
    public:
        ServerIndex index_;
        asio::ip::address innerAddr_;
        asio::ip::address extAdder_;
    };

}


