#include <base/BaseServer.hpp>
#include <iostream>
int main(int argc, char** argv) {

	Flow::BaseServer server;
  if(server.loadOptions(argc, argv))
  	server.initAction();
	return 0;
}
