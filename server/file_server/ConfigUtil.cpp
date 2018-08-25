//
// Created by quran on 2018/8/24.
//

#include "ConfigUtil.hpp"

namespace Flow::FileServer {
    void ConfigUtil::AddAddress(const char* ip, uint16_t port) {
        IM::BaseDefine::IpAddr addr;
        addr.set_ip(ip);
        addr.set_port(port);
        addrs_.push_back(addr);
    }
}