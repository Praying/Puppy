/*
 * Created by Praying on 2018/3/1.
 * Copyright (c) 2018 Flow. All rights reserved.
 */

#include <base/common/Util.hpp>
#include <Base/common/CLock.hpp>
#include <base/network/Packet.hpp>
#include <chrono>

namespace Flow
{
    namespace Util
    {
        uint64_t get_tick_count(){
            std::chrono::time_point<std::chrono::system_clock,std::chrono::milliseconds> tp = std::chrono::time_point_cast<std::chrono::milliseconds>(std::chrono::system_clock::now());
            auto tmp=std::chrono::duration_cast<std::chrono::milliseconds>(tp.time_since_epoch());
            return  tmp.count();
        }

        bool stringToBool(std::string const &str)
        {
            std::string lowerStr = str;
            std::transform(str.begin(), str.end(), lowerStr.begin(), [](char c)
            { return char(::tolower(c)); });
            return lowerStr == "1" || lowerStr == "true" || lowerStr == "yes";
        }


        boost::program_options::variables_map getConsoleArguments(int argc, char **argv)
        {
            boost::program_options::options_description all("Allowed options");
            all.add_options()("help,h", "print usage message")
                    ("version,v", "print version build info");
            //    ("config,c", boost::program_options::value<boost::filesystem::path>(&configFile), "use <arg> as configuration file");

            ///TODO win32 需要特殊处理

            boost::program_options::variables_map vm;
            try
            {
                store(boost::program_options::command_line_parser(argc, argv).options(all).allow_unregistered().run(), vm);
                notify(vm);
            }
            catch (std::exception &e)
            {
                std::cerr << e.what() << "\n";
            }
            if (vm.count("help"))
            {
                std::cout << all << "\n";
            }
            else if (vm.count("version"))
            {
                std::cout << "0.0.1" << "\n";
            }

            return vm;
        }

        int encodeMsg(const uint16_t type, const uint16_t msgID, const char *strData, const uint32_t dataLen, MessageBuffer& buffer)
        {
            MsgHead head;
            head.setMsgID(msgID);
            head.setMsgType(type);
            head.setBodyLength(dataLen);
            char headArray[MsgHead::HEAD_LENGTH] ={0};
            head.encode(headArray);
            buffer.reset();
            buffer.resize(MsgHead::HEAD_LENGTH + dataLen);
            buffer.write(headArray, MsgHead::HEAD_LENGTH);
            buffer.write(strData, dataLen);
            return head.getBodyLength()+MsgHead::HEAD_LENGTH;
        }

        CRefObject::CRefObject()
        {
            m_lock = NULL;
            m_refCount = 1;
        }

        CRefObject::~CRefObject()
        {

        }

        void CRefObject::AddRef()
        {
            if (m_lock)
            {
                m_lock->lock();
                m_refCount++;
                m_lock->unlock();
            }
            else
            {
                m_refCount++;
            }
        }

        void CRefObject::ReleaseRef()
        {
            if (m_lock)
            {
                m_lock->lock();
                m_refCount--;
                if (m_refCount == 0)
                {
                    delete this;
                    return;
                }
                m_lock->unlock();
            }
            else
            {
                m_refCount--;
                if (m_refCount == 0)
                    delete this;
            }
        }

        void writePid()
        {
            uint32_t curPid;
#ifdef _WIN32
            curPid = (uint32_t) GetCurrentProcess();
#else
            curPid = (uint32_t) getpid();
#endif
            FILE* f = fopen("server.pid", "w");
            assert(f);
            char szPid[32];
            snprintf(szPid, sizeof(szPid), "%d", curPid);
            fwrite(szPid, strlen(szPid), 1, f);
            fclose(f);
        }


    }
}
