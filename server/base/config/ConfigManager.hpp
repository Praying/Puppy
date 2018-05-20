/*
 * Created by Praying on 2018/2/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <vector>
#include <memory>
#include <folly/Singleton.h>

namespace Flow
{

    class ConfigManager
    {
        ConfigManager() = default;

        ConfigManager(const ConfigManager &) = delete;

        ConfigManager &operator=(const ConfigManager &) = delete;

        ~ConfigManager() = default;

        friend class folly::Singleton<ConfigManager>;

    public:
        bool loadInitial(const std::string &file, std::vector<std::string> args, std::string &error);
        bool loadInitial(const std::string &file, std::string &error);

        static std::shared_ptr<ConfigManager> instance();

        bool reload(std::string &error);

        std::string getStringDefault(const std::string &name, const std::string &def) const;

        bool getBoolDefault(const std::string &name, bool def) const;

        int getIntDefault(const std::string &name, int def) const;

        float getFloatDefault(const std::string &name, float def) const;

        const std::string &getFileName();

        const std::vector<std::string> &getArguments() const;

        std::vector<std::string> getKeysByString(const std::string &name);

    private:
        template<typename T>
        T getValueDefault(const std::string &name, T def) const;
    };

#define sConfigManager ConfigManager::instance()
}


