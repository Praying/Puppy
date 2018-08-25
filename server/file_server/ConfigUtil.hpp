//
// Created by quran on 2018/8/24.
//

#pragma once

#include <base/common/Singleton.hpp>
#include <cstdint>
#include <list>
#include <base/proto/ProtosGen/IM.BaseDefine.pb.h>

namespace Flow::FileServer {

    class ConfigUtil : public Singleton<ConfigUtil> {
    public:
        ~ConfigUtil() {}

        void AddAddress(const char *ip, uint16_t port);

        const std::list<IM::BaseDefine::IpAddr> &GetAddressList() const {
            return addrs_;
        }

        void SetTaskTimeout(uint32_t timeout) { task_timeout_ = timeout; }

        uint32_t GetTaskTimeout() const { return task_timeout_; }

    private:
        friend class Singleton<ConfigUtil>;

        ConfigUtil()
                : task_timeout_(3600) {}

        std::list<IM::BaseDefine::IpAddr> addrs_;
        uint32_t task_timeout_;
    };

}

