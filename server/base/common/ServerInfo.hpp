/*
 * Created by Praying on 2018/3/24.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <string>
#include <vector>

namespace Flow
{

    class ServerInfo
    {
    public:
        /// Constructor
        ServerInfo();

        /// Returns the hostname of this machine
        const std::string& serverName()const{return name_;}

        /// Returns a textual desciption of this machine's CPU
        const std::string& cpuInfo() const{return cpuInfo_;}

        /// Returns a vector of CPU speeds for the CPU cores in this machine
        const std::vector<float>& cpuSpeed() const {return cpuSpeeds_;}

        /// Returns a textual description of this machine's RAM
        const std::string& memInfo() const {return memInfo_;}

        /// Returns the number of bytes of total RAM
        const uint64_t memTotal() const {return memTotal_;}

        /// Returns the number of bytes of process-used RAM
        const uint64_t memUsed() const {return memUsed_;}

        /// Request an update of the RAM statistics
        void updateMem();
    private:

#if !defined(_WIN32)
        void fetchLinuxCpuInfo();
        void fetchLinuxMemInfo();
#endif
        std::string name_;
        std::string cpuInfo_;
        std::vector<float> cpuSpeeds_;
        std::string memInfo_;
        uint64_t memTotal_;
        uint64_t memUsed_;
    };

}


