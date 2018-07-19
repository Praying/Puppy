#include <base/BaseServer.hpp>
#include <iostream>
int main(int argc, char** argv) {

	Flow::BaseServer server;
	if(!server.initAction(argc,argv))
		return 0;
	server.runAction();
	server.finishAction();
	return 0;
}
