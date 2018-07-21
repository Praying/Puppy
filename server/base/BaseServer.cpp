//
// Created by quran on 2018/7/14.
//

#include "BaseServer.hpp"
#include <boost/program_options.hpp>
#include <iostream>
#include <fstream>
#include "base/network/ChannelManager.hpp"
#include <base/common/Define.hpp>
#include <google/protobuf/stubs/common.h>
#include <base/config/ConfigManager.hpp>
#include <glog/logging.h>

namespace Flow {

    namespace {
        void AbortHandler(int /*sigval*/) {
            *((volatile int *) NULL) = 0;
            exit(1);
        }

        namespace fs = boost::filesystem;
        using namespace boost::program_options;

        variables_map GetConsoleArguments(int argc, char **argv, fs::path &configFile, std::string &configService) {
            // Silences warning about configService not be used if the OS is ·not Windows
            (void) configService;

            options_description all("Allowed options");
            all.add_options()
                    ("help,h", "print usage message")
                    ("version,v", "print version build info")
                    ("config,c", value<fs::path>(&configFile),
                     "use <arg> as configuration file");
#ifdef _WIN32
            options_description win("Windows platform specific options");
    win.add_options()
        ("service,s", value<std::string>(&configService)->default_value(""), "Windows service options: [install | uninstall]")
        ;

    all.add(win);
#endif
            variables_map vm;
            try {
                store(command_line_parser(argc, argv).options(all).allow_unregistered().run(), vm);
                notify(vm);
            }
            catch (std::exception &e) {
                std::cerr << e.what() << "\n";
            }

            if (vm.count("help")) {
                std::cout << all << "\n";
            } else if (vm.count("version")) {
                std::cout << "version 0.0.1" << "\n";
            }

            return vm;
        }
    }

    BaseServer::BaseServer() {
        //  initConfigOptions();
    }

    bool BaseServer::initAction(int argc, char **argv) {
        google::InitGoogleLogging(argv[0]);
        namespace fs = boost::filesystem;
        signal(SIGABRT, &AbortHandler);
        auto configFile = fs::absolute(getConfigFileName());
        std::string configService;
        auto vm = GetConsoleArguments(argc, argv, configFile, configService);
        if (vm.count("help") || vm.count("version"))
            return false;
        GOOGLE_PROTOBUF_VERIFY_VERSION;

#ifdef _WIN32
        if (configService.compare("install") == 0)
            return WinServiceInstall() ? 0 : 1;
        else if (configService.compare("uninstall") == 0)
            return WinServiceUninstall() ? 0 : 1;
        else if (configService.compare("run") == 0)
            return WinServiceRun() ? 0 : 0;
#endif
        std::string configError;
        if (!sConfigManager->loadInitial(configFile.generic_string(),
                                        std::vector<std::string>(argv, argv + argc),
                                        configError)) {
            std::cerr << "Error in config file: %s" << configError << "\n";
            return false;
        }

        ioContext_ = std::make_shared<asio::io_context>();

        asio::signal_set signals(*ioContext_, SIGINT, SIGTERM);
#ifdef _WIN32
        signals.add(SIGBREAK);
#endif

        signals.async_wait([this](asio::error_code ec,int sig){this->signalHandler(ec,sig);});
        int numThreads = sConfigManager->getIntDefault("ThreadPool", 1);
        threadPool_ =std::shared_ptr<std::vector<std::thread> >(new std::vector<std::thread>, [this](std::vector<std::thread> *del) {
            this->ioContext_->stop();
            for (std::thread &thr : *del) {
                thr.join();
            }
            delete del;
        });
        if (numThreads < 1)
            numThreads = 1;
        for (int i = 0; i < numThreads; ++i) {
            threadPool_->push_back(std::thread([this]() { this->ioContext_->run(); }));
        }

        //Launch the world server listener socket
        uint16_t clientPort = uint16_t(sConfigManager->getIntDefault("ClientPort", 9999));
        uint16_t serverPort = uint16_t(sConfigManager->getIntDefault("ServerPort", 9998));
        std::string listener = sConfigManager->getStringDefault("BindIP", "0.0.0.0");
        int networkThreads = sConfigManager->getIntDefault("Network.Threads",1);
        if(networkThreads <= 0){
            return false;
        }
        if(!sChannelManager->startNetwork(*ioContext_,listener,clientPort, serverPort,numThreads)){
            LOG(ERROR) << "Start network failed";
            return false;
        }


        initOpCodeTable();
        LOG(INFO)<<"Server start successfully!!!";
        return true;
    }

    void BaseServer::runAction() {
        updateLoop();
    }

    void BaseServer::finishAction() {
        threadPool_.reset();
        google::protobuf::ShutdownProtobufLibrary();
    }

    bool BaseServer::initOpCodeTable() {
        return false;
    }

    bool BaseServer::initNetwork() {
        return true;
    }

    void BaseServer::updateLoop() {
        while(!stopEvent_){
            //TODO
        }
    }

    bool BaseServer::finishNetwork() {
        return false;
    }

    void BaseServer::initConfigOptions() {
        configOptionsDesc_.add_options()
                ("help", "Displays this help dialog.")
                ("BindAddress", boost::program_options::value<std::string>()->default_value("127.0.0.1"),
                 "Network listen address.")
                ("BindPort", boost::program_options::value<uint16_t>()->default_value(10000),
                 "Port the server listens for messages on.")
                ("ServiceMessageHeap", boost::program_options::value<uint32_t>()->default_value(8192), "")
                ("GlobalMessageHeap", boost::program_options::value<uint32_t>()->default_value(8192), "")
                ("DBServer", boost::program_options::value<std::string>()->default_value("localhost"),
                 "Address of the MySQL Server.")
                ("DBPort", boost::program_options::value<uint16_t>()->default_value(3306), "Port of the MySQL Server.")
                ("DBName", boost::program_options::value<std::string>()->default_value("swganh"),
                 "Name of the MySQL database schema.")
                ("DBUser", boost::program_options::value<std::string>()->default_value("root"),
                 "Username of the database user account.")
                ("DBPass", boost::program_options::value<std::string>()->default_value(""),
                 "Password of the database user account.")
                ("DBMinThreads", boost::program_options::value<uint32_t>()->default_value(4),
                 "Minimum number of threads used for database work.")
                ("DBMaxThreads", boost::program_options::value<uint32_t>()->default_value(16),
                 "Maximum number of threads used for database work.")
                ("ReliablePacketSizeServerToServer", boost::program_options::value<uint16_t>()->default_value(1400),
                 "size of Packets for server server communication")
                ("UnreliablePacketSizeServerToServer", boost::program_options::value<uint16_t>()->default_value(1400),
                 "size of Packets for server server communication")
                ("ReliablePacketSizeServerToClient", boost::program_options::value<uint16_t>()->default_value(496),
                 "size of Packets for server client communication")
                ("UnreliablePacketSizeServerToClient", boost::program_options::value<uint16_t>()->default_value(496),
                 "size of Packets for server client communication")
                ("ServerPacketWindowSize", boost::program_options::value<uint32_t>()->default_value(800), "")
                ("ClientPacketWindowSize", boost::program_options::value<uint32_t>()->default_value(80), "")
                ("UdpBufferSize", boost::program_options::value<uint32_t>()->default_value(4096), "Kernel UDP Buffer")
                ("DBGlobalSchema", boost::program_options::value<std::string>()->default_value("swganh_static"), "")
                ("DBGalaxySchema", boost::program_options::value<std::string>()->default_value("swganh"), "")
                ("DBConfigSchema", boost::program_options::value<std::string>()->default_value("swganh_config"), "");

    }

    bool BaseServer::loadOptions(uint32_t argc, char **argv) {
        namespace popt = boost::program_options;
        popt::store(
                popt::parse_command_line(argc, const_cast<const char *const *>(argv), configOptionsDesc_),
                configVarMap_);
        popt::notify(configVarMap_);
        if (configVarMap_.count("help")) {
            std::cout << configOptionsDesc_ << std::endl;
            return false;
        }
        return true;
    }

    bool BaseServer::loadOptions(std::list<std::string> configFiles) {
        // Iterate through the configuration files
        // that are to be loaded. If a configuration file
        // is missing, throw a runtime_error.
        std::for_each(configFiles.begin(), configFiles.end(), [=](const std::string &filename) {
            std::ifstream config_file(filename);
            if (!config_file)
                throw std::runtime_error("Could not open configuration file.");
            else
                boost::program_options::store(
                        boost::program_options::parse_config_file(config_file, configOptionsDesc_, true),
                        configVarMap_);
        });

        boost::program_options::notify(configVarMap_);

        // The help argument has been flagged, display the
        // server options and throw a runtime_error exception
        // to stop server startup.
        if (configVarMap_.count("help")) {
            std::cout << configOptionsDesc_ << std::endl;
            return false;
        }
        return true;
    }

    bool BaseServer::loadOptions(uint32_t argc, char **argv, std::list<std::string> configFiles) {
        boost::program_options::store(
                boost::program_options::parse_command_line(argc, const_cast<const char *const *>(argv),
                                                           configOptionsDesc_), configVarMap_);

        // Iterate through the configuration files
        // that are to be loaded. If a configuration file
        // is missing, throw a runtime_error.
        std::for_each(configFiles.begin(), configFiles.end(), [=](const std::string &filename) {
            std::ifstream configFile(filename);
            if (!configFile)
                throw std::runtime_error("Could not open configuration file.");
            else
                boost::program_options::store(
                        boost::program_options::parse_config_file(configFile, configOptionsDesc_, true), configVarMap_);
        });

        boost::program_options::notify(configVarMap_);

        // The help argument has been flagged, display the
        // server options and throw a runtime_error exception
        // to stop server startup.
        if (configVarMap_.count("help")) {
            std::cout << configOptionsDesc_ << std::endl;
            return false;
        }
        return true;
    }


    void BaseServer::initConfig() {

    }


    std::string BaseServer::getConfigFileName() {
        return std::__cxx11::string();
    }

    void BaseServer::signalHandler(const asio::error_code &error, int) {
        if (!error)
            stopNow();
    }

    void BaseServer::stopNow() {
        stopEvent_ = true;
    }
}
