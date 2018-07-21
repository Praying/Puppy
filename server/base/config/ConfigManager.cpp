/*
 * Created by Praying on 2018/2/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include <base/config/ConfigManager.hpp>
#include <base/common/Util.hpp>
#include <base/helper/DebugHelper.hpp>
#include <boost/property_tree/ini_parser.hpp>
#include <algorithm>
#include <memory>
#include <mutex>

namespace Flow
{
    namespace bpt = boost::property_tree;

    namespace
    {
        std::string filename_;
        std::vector<std::string> args_;
        bpt::ptree config_;
        std::mutex configLock_;
    }


    FLOW_SINGLETON_IMP(ConfigManager);

    template<typename T>
    T ConfigManager::getValueDefault(const std::string &name, T def) const
    {
        try
        {
            return config_.get<T>(bpt::ptree::path_type(name, '/'));
        } catch (bpt::ptree_bad_path)
        {
//            FLOW_WARN("[server.loading]Missing name {} in config file {}, add \"{} = {}\" to this file", name.c_str(),
//                  filename_.c_str(), name.c_str(), std::to_string(def).c_str());

        } catch (bpt::ptree_bad_data)
        {
     /*       FLOW_WARN("[server.loading]Bad value defined for name {} in config file {}, going to use {} instead",
                  name.c_str(), filename_.c_str(), std::to_string(def).c_str());
     */
        }
        return def;
    }

    template<>
    std::string ConfigManager::getValueDefault<std::string>(const std::string &name, std::string def) const
    {
        try
        {
            return config_.get<std::string>(bpt::ptree::path_type(name, '/'));
        } catch (bpt::ptree_bad_path)
        {
        /*    FLOW_WARN("[server.loading]Missing name {} in config file {}, add \"{} = {}\" to this file", name.c_str(),
                  filename_.c_str(), name.c_str(), def.c_str());
        */} catch (bpt::ptree_bad_data)
        {
          /*  FLOW_WARN("[server.loading]Bad value defined for name {} in config file {}, going to use {} instead",
                  name.c_str(), filename_.c_str(), def.c_str());
       */ }
        return def;
    }

    bool ConfigManager::loadInitial(const std::string &file, std::vector<std::string> args, std::string &error)
    {
        std::lock_guard<std::mutex> lock(configLock_);
        filename_ = file;
        args_ = args;

        try
        {
            bpt::ptree fullTree;
            bpt::ini_parser::read_ini(file, fullTree);
            if (fullTree.empty())
            {
                error = "empty file (" + file + ")";
                return false;
            }
            config_ = fullTree.begin()->second;
        } catch (bpt::ini_parser_error const &e)
        {
            if (e.line() == 0)
                error = e.message() + " (" + e.filename() + ")";
            else
                error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
            return false;
        }
        return true;

    }

    bool ConfigManager::reload(std::string &error)
    {
        return loadInitial(filename_, std::move(args_), error);
    }


    std::string ConfigManager::getStringDefault(const std::string &name, const std::string &def) const
    {
        std::string val = getValueDefault(name, def);
        val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
        return val;
    }

    bool ConfigManager::getBoolDefault(const std::string &name, bool def) const
    {
        std::string val = getValueDefault(name, std::string(def ? "1" : "0"));
        val.erase(std::remove(val.begin(), val.end(), '"'), val.end());
        return Util::stringToBool(val);
    }

    int ConfigManager::getIntDefault(const std::string &name, int def) const
    {
        return getValueDefault(name, def);
    }

    float ConfigManager::getFloatDefault(const std::string &name, float def) const
    {
        return getValueDefault(name, def);
    }

    const std::string &ConfigManager::getFileName()
    {
        std::lock_guard<std::mutex> lock(configLock_);
        return filename_;
    }

    const std::vector<std::string> &ConfigManager::getArguments() const
    {
        return args_;
    }

    std::vector<std::string> ConfigManager::getKeysByString(const std::string &name)
    {
        std::lock_guard<std::mutex> lock(configLock_);
        std::vector<std::string> keys;
        for (bpt::ptree::value_type const &child : config_)
        {
            if (child.first.compare(0, name.length(), name) == 0)
            {
                keys.push_back(child.first);
            }
        }
        return keys;
    }

    bool ConfigManager::loadInitial(const std::string &file, std::string &error)
    {
        std::lock_guard<std::mutex> lock(configLock_);
        filename_ = file;

        try
        {
            bpt::ptree fullTree;
            bpt::ini_parser::read_ini(file, fullTree);
            if (fullTree.empty())
            {
                error = "empty file (" + file + ")";
                return false;
            }
            config_ = fullTree.begin()->second;
        } catch (bpt::ini_parser_error const &e)
        {
            if (e.line() == 0)
                error = e.message() + " (" + e.filename() + ")";
            else
                error = e.message() + " (" + e.filename() + ":" + std::to_string(e.line()) + ")";
            return false;
        }
        return true;
    }


}