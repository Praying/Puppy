#include <iostream>
#include "CachePool.hpp"
#include "DBPool.hpp"
#include <signal.h>
#include <cstring>
#include <glog/logging.h>

using namespace Flow;

#define VERSION "0.0.1"

int main(int argc, char** argv)
{

    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
        printf("Server Version: DBProxyServer/%s\n", VERSION);
        printf("Server Build: %s %s\n", __DATE__, __TIME__);
        return 0;
    }

    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    google::InitGoogleLogging(argv[0]);

    // 1. 初始化Redis连接
    CacheManager* pCacheManager = CacheManager::getInstance();
    if (!pCacheManager) {
        LOG(ERROR)<<"CacheManager init failed";
        return -1;
    }
    //  2. 初始化MySql连接
    CDBManager* pDBManager = CDBManager::getInstance();
    if (!pDBManager) {
        LOG(ERROR)<<"DBManager init failed";
        return -1;
    }
    // 3. 启动任务队列，用于处理任务
    // 4. 启动从Mysql同步数据到Redis工作
    // 5. 在端口10600上启动监听，监听新连接
    // 6. 主线程进入循环，监听新连接的到来以及处理新连接上的数据收发
    return 0;
}