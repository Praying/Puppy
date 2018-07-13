//
// Created by quran on 2018/7/14.
//

#ifndef SERVER_BASESERVER_HPP
#define SERVER_BASESERVER_HPP

#include <boost/program_options/options_description.hpp>
#include <boost/program_options/variables_map.hpp>

namespace Flow{
    class BaseServer {
    public:
        BaseServer();

        void loadOptions(uint32_t argc, char** argv);
        void loadOptions(std::list<std::string> configFiles);
        void loadOptions(uint32_t argc, char **argv, std::list<std::string> configFiles);
    public:
        virtual void initAction();
        virtual void runAction();
        virtual void finishAction();

    protected:
        virtual bool initOpCodeTable();
        virtual bool initNetwork();
        virtual void update();
        virtual bool finishNetwork();
        virtual void initConfigOptions();

    private:
        boost::program_options::options_description configOptionsDesc_;
        boost::program_options::variables_map configVarMap_;
    };
}



#endif //SERVER_BASESERVER_HPP
