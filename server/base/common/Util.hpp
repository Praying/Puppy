/*
 * Created by Praying on 2018/2/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#pragma once

#include <string>
#include <iostream>
#include <boost/program_options.hpp>
#include <boost/filesystem/path.hpp>
#include <boost/filesystem.hpp>
#include <base/network/MessageBuffer.hpp>
#include "CLock.hpp"

namespace Flow {
    namespace Util {
        bool stringToBool(std::string const &str);

        boost::program_options::variables_map getConsoleArguments(int argc, char **argv);

        int encodeMsg(const uint16_t type, const uint16_t msgID, const char *strData, const uint32_t dataLen,
                      MessageBuffer &buffer);

        class CStrExplode {
        public:
            CStrExplode(char *str, char seperator) {
                m_item_cnt = 1;
                char *pos = str;
                while (*pos) {
                    if (*pos == seperator) {
                        m_item_cnt++;
                    }

                    pos++;
                }

                m_item_list = new char *[m_item_cnt];

                int idx = 0;
                char *start = pos = str;
                while (*pos) {
                    if (pos != start && *pos == seperator) {
                        uint32_t len = pos - start;
                        m_item_list[idx] = new char[len + 1];
                        strncpy(m_item_list[idx], start, len);
                        m_item_list[idx][len] = '\0';
                        idx++;

                        start = pos + 1;
                    }

                    pos++;
                }

                uint32_t len = pos - start;
                if (len != 0) {
                    m_item_list[idx] = new char[len + 1];
                    strncpy(m_item_list[idx], start, len);
                    m_item_list[idx][len] = '\0';
                }
            }

            virtual ~CStrExplode() {
                for (uint32_t i = 0; i < m_item_cnt; i++) {
                    delete[] m_item_list[i];
                }

                delete[] m_item_list;
            }


            uint32_t GetItemCnt() { return m_item_cnt; }

            char *GetItem(uint32_t idx) { return m_item_list[idx]; }

        private:
            uint32_t m_item_cnt;
            char **m_item_list;
        };

        class CRefObject {
        public:
            CRefObject();

            virtual ~CRefObject();

            void SetLock(CLock *lock) { m_lock = lock; }

            void AddRef();

            void ReleaseRef();

        private:
            int m_refCount;
            CLock *m_lock;
        };

        uint64_t get_tick_count();

        void writePid();

        extern  uint32_t string2int(const std::string &value);

        extern  std::string int2string(uint32_t user_id);

        extern  void replace_mark(std::string &str, uint32_t new_value, uint32_t &begin_pos);

        extern  void replace_mark(std::string &str, std::string &new_value, uint32_t &begin_pos);

        const char*  memfind(const char *src_str,size_t src_len, const char *sub_str, size_t sub_len, bool flag=true);
    }
}