//
// Created by quran on 2018/8/24.
//

#ifndef SERVER_CCONFIGFILEREADER_HPP
#define SERVER_CCONFIGFILEREADER_HPP

#include <string>
#include <map>

namespace Flow::PushServer {
    class CConfigFileReader {
    public:
        CConfigFileReader(const char *filename);

        ~CConfigFileReader();

        char *GetConfigName(const char *name);

    private:
        void _LoadFile(const char *filename);

        void _ParseLine(char *line);

        char *_TrimSpace(char *name);

        bool m_load_ok;
        std::map <std::string, std::string> *m_config_map;
    };
}

#endif //SERVER_CCONFIGFILEREADER_HPP
