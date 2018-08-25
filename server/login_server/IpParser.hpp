//
// Created by quran on 2018/8/24.
//

#pragma once

namespace Flow::LoginServer {

    class IpParser
    {
    public:
        IpParser();
        virtual ~IpParser();
        bool isTelcome(const char* ip);
    };

}

