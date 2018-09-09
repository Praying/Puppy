#include <base/BaseServer.hpp>
#include <iostream>
#include <base/common/OSType.hpp>
#include <glog/logging.h>
#include <base/network/Netlib.hpp>
#include <base/config/ConfigFileReader.hpp>
#include "CRouteConn.hpp"

using namespace Flow;
// this callback will be replaced by imconn_callback() in OnConnect()
void route_serv_callback(void* callback_data, uint8_t msg, uint32_t handle, void* pParam)
{
	if (msg == NETLIB_MSG_CONNECT)
	{
		CRouteConn* pConn = new CRouteConn();
		pConn->OnConnect(handle);
	}
	else
	{
		LOG(ERROR)<<"!!!error msg: %d "<< msg;
	}
}

int main(int argc, char** argv) {

	if ((argc == 2) && (strcmp(argv[1], "-v") == 0)) {
		printf("Server Version: RouteServer/%s\n", VERSION);
		printf("Server Build: %s %s\n", __DATE__, __TIME__);
		return 0;
	}

	signal(SIGPIPE, SIG_IGN);
	srand(time(NULL));
	google::InitGoogleLogging(argv[0]);
	ConfigFileReader config_file("routeserver.conf");

	char* listen_ip = config_file.GetConfigName("ListenIP");
	char* str_listen_msg_port = config_file.GetConfigName("ListenMsgPort");

	if (!listen_ip || !str_listen_msg_port) {
		LOG(ERROR)<<"config item missing, exit... ";
		return -1;
	}

	uint16_t listen_msg_port = atoi(str_listen_msg_port);

	int ret = netlib_init();

	if (ret == NETLIB_ERROR)
		return ret;

	CStrExplode listen_ip_list(listen_ip, ';');
	for (uint32_t i = 0; i < listen_ip_list.GetItemCnt(); i++) {
		ret = netlib_listen(listen_ip_list.GetItem(i), listen_msg_port, route_serv_callback, NULL);
		if (ret == NETLIB_ERROR)
			return ret;
	}

	printf("server start listen on: %s:%d\n", listen_ip,  listen_msg_port);

	init_routeconn_timer_callback();

	printf("now enter the event loop...\n");

	writePid();
	netlib_eventloop();

	return 0;
}
