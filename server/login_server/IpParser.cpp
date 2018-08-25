//
// Created by quran on 2018/8/24.
//

#include <base/common/Util.hpp>
#include "IpParser.hpp"

namespace Flow::LoginServer {
    IpParser::IpParser()
    {
    }

    IpParser::~IpParser()
    {

    }

    bool IpParser::isTelcome(const char *pIp)
    {
        if(!pIp)
        {
            return false;
        }
        Util::CStrExplode strExp((char*)pIp,'.');
        if(strExp.GetItemCnt() != 4)
        {
            return false;
        }
        return true;
    }
}