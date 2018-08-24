#include <iostream>
#include "CachePool.hpp"
#include "DBPool.hpp"
#include "CProxyConn.hpp"
#include <signal.h>
#include <cstring>
#include <glog/logging.h>
#include <base/config/ConfigFileReader.hpp>
#include <base/network/Netlib.hpp>
#include <base/common/Util.hpp>
#include <db_proxy_server/business/CUserModel.hpp>
#include <db_proxy_server/business/CRelationModel.hpp>
#include <db_proxy_server/business/MessageModel.hpp>
#include <db_proxy_server/business/CGroupMessageModel.hpp>
#include <db_proxy_server/business/CAudioModel.hpp>
#include <db_proxy_server/business/CGroupModel.hpp>
#include <db_proxy_server/business/CSessionModel.hpp>
#include <db_proxy_server/business/CFileModel.hpp>
#include <base/common/OSType.hpp>
namespace Flow{
    std::string strAudioEnc;
}

using namespace Flow;


// this callback will be replaced by imconn_callback() in OnConnect()
void proxy_serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
    if (msg == NETLIB_MSG_CONNECT)
    {
        CProxyConn* pConn = new CProxyConn();
        pConn->OnConnect(handle);
    }
    else
    {
        LOG(ERROR)<<"!!!error msg: "<< msg;
    }
}

int main(int argc, char **argv) {

    if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
        printf("Server Version: DBProxyServer/%s\n", VERSION);
        printf("Server Build: %s %s\n", __DATE__, __TIME__);
        return 0;
    }

    signal(SIGPIPE, SIG_IGN);
    srand(time(NULL));

    google::InitGoogleLogging(argv[0]);

    // 1. 初始化Redis连接
    CacheManager *pCacheManager = CacheManager::getInstance();
    if (!pCacheManager) {
        LOG(ERROR) << "CacheManager init failed";
        return -1;
    }
    //  2. 初始化MySql连接
    CDBManager *pDBManager = CDBManager::getInstance();
    if (!pDBManager) {
        LOG(ERROR) << "DBManager init failed";
        return -1;
    }
    std::cout << "db init success" << std::endl;
    // 3. 启动任务队列，用于处理任务

    // 主线程初始化单例，不然在工作线程可能会出现多次初始化
    if (!CAudioModel::getInstance()) {
        return -1;
    }

    if (!CGroupMessageModel::getInstance()) {
        return -1;
    }

    if (!CGroupModel::getInstance()) {
        return -1;
    }

    if (!CMessageModel::getInstance()) {
        return -1;
    }

    if (!CSessionModel::getInstance()) {
        return -1;
    }

    if(!CRelationModel::getInstance())
    {
        return -1;
    }

    if (!CUserModel::getInstance()) {
        return -1;
    }

    if (!CFileModel::getInstance()) {
        return -1;
    }

    ConfigFileReader config_file("dbproxyserver.conf");

    char *listen_ip = config_file.GetConfigName("ListenIP");
    char *str_listen_port = config_file.GetConfigName("ListenPort");
    char *str_thread_num = config_file.GetConfigName("ThreadNum");
    char *str_file_site = config_file.GetConfigName("MsfsSite");
    char *str_aes_key = config_file.GetConfigName("aesKey");

    if (!listen_ip || !str_listen_port || !str_thread_num || !str_file_site || !str_aes_key) {
        LOG(ERROR) << "missing ListenIP/ListenPort/ThreadNum/MsfsSite/aesKey, exit...";
        return -1;
    }

    uint16_t listen_port = atoi(str_listen_port);
    uint32_t thread_num = atoi(str_thread_num);

    int ret = netlib_init();

    if (ret == NETLIB_ERROR)
        return ret;
    Util::CStrExplode listen_ip_list(listen_ip, ';');
    for (uint32_t i = 0; i < listen_ip_list.GetItemCnt(); i++) {
        ret = netlib_listen(listen_ip_list.GetItem(i), listen_port, proxy_serv_callback, NULL);
        if (ret == NETLIB_ERROR)
            return ret;
    }

    printf("server start listen on: %s:%d\n", listen_ip,  listen_port);
    printf("now enter the event loop...\n");
    Util::writePid();
    netlib_eventloop(10);


    // 4. 启动从Mysql同步数据到Redis工作
    // 5. 在端口10600上启动监听，监听新连接
    // 6. 主线程进入循环，监听新连接的到来以及处理新连接上的数据收发
    return 0;
}