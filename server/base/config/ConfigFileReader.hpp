//
// Created by quran on 2018/8/17.
//

#ifndef SERVER_CONFIGFILEREADER_HPP
#define SERVER_CONFIGFILEREADER_HPP

#include <string>
#include <map>

namespace Flow{
    class ConfigFileReader
    {
    public:
        ConfigFileReader(const char* filename);
        ~ConfigFileReader();

        char* GetConfigName(const char* name);
        int SetConfigValue(const char* name, const char*  value);
    private:
        void _LoadFile(const char* filename);
        int _WriteFile(const char *filename = NULL);
        void _ParseLine(char* line);
        char* _TrimSpace(char* name);

        bool m_load_ok;
        std::map<std::string, std::string> m_config_map;
        std::string m_config_file;
    };

}



#endif //SERVER_CONFIGFILEREADER_HPP
