//
// Created by quran on 2018/7/14.
//

#include "BaseServer.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "base/network/ChannelManager.hpp"

namespace Flow{


    BaseServer::BaseServer() {
        initConfigOptions();
    }

    void BaseServer::initAction() {
        initOpCodeTable();
        initNetwork();
    }

    void BaseServer::runAction() {
        update();
    }

    void BaseServer::finishAction() {

    }

    bool BaseServer::initOpCodeTable() {
        return false;
    }

    bool BaseServer::initNetwork() {
   //     std::string ip = configVarMap_["BindAddress"].as<std::string>();
   //     uint32_t port = configVarMap_["BindPort"].as<uint16_t >();
    }

    void BaseServer::update() {

    }

    bool BaseServer::finishNetwork() {
        return false;
    }

    void BaseServer::initConfigOptions() {
        configOptionsDesc_.add_options()
                ("help", "Displays this help dialog.")
                ("BindAddress", boost::program_options::value<std::string>()->default_value("127.0.0.1"), "Network listen address.")
                ("BindPort", boost::program_options::value<uint16_t>()->default_value(10000), "Port the server listens for messages on.")
                ("ServiceMessageHeap", boost::program_options::value<uint32_t>()->default_value(8192), "")
                ("GlobalMessageHeap", boost::program_options::value<uint32_t>()->default_value(8192), "")
                ("DBServer", boost::program_options::value<std::string>()->default_value("localhost"), "Address of the MySQL Server.")
                ("DBPort", boost::program_options::value<uint16_t>()->default_value(3306), "Port of the MySQL Server.")
                ("DBName", boost::program_options::value<std::string>()->default_value("swganh"), "Name of the MySQL database schema.")
                ("DBUser", boost::program_options::value<std::string>()->default_value("root"), "Username of the database user account.")
                ("DBPass", boost::program_options::value<std::string>()->default_value(""), "Password of the database user account.")
                ("DBMinThreads", boost::program_options::value<uint32_t>()->default_value(4), "Minimum number of threads used for database work.")
                ("DBMaxThreads", boost::program_options::value<uint32_t>()->default_value(16), "Maximum number of threads used for database work.")
                ("ReliablePacketSizeServerToServer", boost::program_options::value<uint16_t>()->default_value(1400), "size of Packets for server server communication")
                ("UnreliablePacketSizeServerToServer", boost::program_options::value<uint16_t>()->default_value(1400), "size of Packets for server server communication")
                ("ReliablePacketSizeServerToClient", boost::program_options::value<uint16_t>()->default_value(496), "size of Packets for server client communication")
                ("UnreliablePacketSizeServerToClient", boost::program_options::value<uint16_t>()->default_value(496), "size of Packets for server client communication")
                ("ServerPacketWindowSize", boost::program_options::value<uint32_t>()->default_value(800), "")
                ("ClientPacketWindowSize", boost::program_options::value<uint32_t>()->default_value(80), "")
                ("UdpBufferSize", boost::program_options::value<uint32_t>()->default_value(4096), "Kernel UDP Buffer")
                ("DBGlobalSchema", boost::program_options::value<std::string>()->default_value("swganh_static"), "")
                ("DBGalaxySchema", boost::program_options::value<std::string>()->default_value("swganh"), "")
                ("DBConfigSchema", boost::program_options::value<std::string>()->default_value("swganh_config"), "");

    }

    void BaseServer::loadOptions(uint32_t argc, char **argv) {
        namespace popt = boost::program_options;
        popt::store(
                popt::parse_command_line(argc,const_cast<const char* const*>(argv), configOptionsDesc_)
                ,configVarMap_);
        popt::notify(configVarMap_);
        if(configVarMap_.count("help")){
            std::cout<< configOptionsDesc_<<std::endl;
            throw std::runtime_error("Help option flagged.");
        }
    }

    void BaseServer::loadOptions(std::list<std::string> configFiles) {
        // Iterate through the configuration files
        // that are to be loaded. If a configuration file
        // is missing, throw a runtime_error.
        std::for_each(configFiles.begin(), configFiles.end(), [=] (const std::string& filename) {
            std::ifstream config_file(filename);
            if(!config_file)
                throw std::runtime_error("Could not open configuration file.");
            else
                boost::program_options::store(boost::program_options::parse_config_file(config_file, configOptionsDesc_, true), configVarMap_);
        });

        boost::program_options::notify(configVarMap_);

        // The help argument has been flagged, display the
        // server options and throw a runtime_error exception
        // to stop server startup.
        if(configVarMap_.count("help"))
        {
            std::cout << configOptionsDesc_ << std::endl;
            throw std::runtime_error("Help option flagged.");
        }
    }

    void BaseServer::loadOptions(uint32_t argc, char **argv, std::list<std::string> configFiles) {
        boost::program_options::store(boost::program_options::parse_command_line(argc, const_cast<const char* const *>(argv), configOptionsDesc_), configVarMap_);

        // Iterate through the configuration files
        // that are to be loaded. If a configuration file
        // is missing, throw a runtime_error.
        std::for_each(configFiles.begin(), configFiles.end(), [=] (const std::string& filename) {
            std::ifstream configFile(filename);
            if(!configFile)
                throw std::runtime_error("Could not open configuration file.");
            else
                boost::program_options::store(boost::program_options::parse_config_file(configFile, configOptionsDesc_, true), configVarMap_);
        });

        boost::program_options::notify(configVarMap_);

        // The help argument has been flagged, display the
        // server options and throw a runtime_error exception
        // to stop server startup.
        if(configVarMap_.count("help"))
        {
            std::cout << configOptionsDesc_ << std::endl;
            throw std::runtime_error("Help option flagged.");
        }
    }
}