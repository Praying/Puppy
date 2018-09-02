#include <iostream>
#include <push_server/timer/BaseTimer.hpp>
#include <signal.h>
#include <csignal>
#include <cstring>
#include <cassert>
#include "CPushApp.hpp"
#include "PushDefine.hpp"

using namespace Flow::PushServer;

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


int main(int argc, const char * argv[]) {
    google::InitGoogleLogging(argv[0]);
    // insert code here...
    printf("start push server...\n");
    signal(SIGPIPE, SIG_IGN);
    if(!CPushApp::GetInstance()->Init()){
        LOG(ERROR)<<"PushServer init failed!!!";
        return 1;
    }

    if (!CPushApp::GetInstance()->Start()){
        LOG(ERROR)<<"PushServer start failed!!!";
        return 1;
    }
    writePid();
    while (true) {
        S_Sleep(1000);
    }
    google::ShutdownGoogleLogging();
    return 0;
}