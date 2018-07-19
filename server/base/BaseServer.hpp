//
// Created by quran on 2018/7/14.
//

#ifndef SERVER_BASESERVER_HPP
#define SERVER_BASESERVER_HPP

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>
#include <boost/filesystem.hpp>
#include <asio/io_context.hpp>
#include <thread>

namespace Flow{
    class BaseServer {
    public:
        BaseServer();

        bool loadOptions(uint32_t argc, char** argv);
        bool loadOptions(std::list<std::string> configFiles);
        bool loadOptions(uint32_t argc, char **argv, std::list<std::string> configFiles);
    public:
        virtual void initConfig();
        virtual bool initAction(int argc, char** argv);
        virtual void runAction();
        virtual void finishAction();
        void stopNow();

    protected:
        virtual bool initOpCodeTable();
        virtual bool initNetwork();
        virtual void updateLoop();
        virtual bool finishNetwork();
        virtual void initConfigOptions();
        virtual std::string getConfigFileName();

        void signalHandler(const asio::error_code& error, int /*SignalNumber*/);

    private:
        boost::program_options::options_description configOptionsDesc_;
        boost::program_options::variables_map configVarMap_;
        std::shared_ptr<asio::io_context> ioContext_;
        std::shared_ptr<std::vector<std::thread>> threadPool_;
        std::atomic_bool stopEvent_ = false;
    };
}



#endif //SERVER_BASESERVER_HPP
