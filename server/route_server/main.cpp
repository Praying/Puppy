/*#include <folly/fibers/Fiber.h>
#include <folly/fibers/EventBaseLoopController.h>
#include <folly/fibers/FiberManagerMap.h>*/
#include <iostream>
int main() {
/*	folly::EventBase evb;
	auto& fiberManager = folly::fibers::getFiberManager(evb);
	folly::fibers::Baton baton;
	fiberManager.addTask([&]{
		std::cout << "Task 1: start " << std::endl;
		baton.wait();
		std::cout << "Task 1: after baton.wait()" << std::endl;
		});
	fiberManager.addTask([&]{
		std::cout << "Task 2: start " << std::endl;
	baton.wait();
	std::cout << "Task 2: after baton.wait()" << std::endl;
		});
	evb.loop();*/
	return 0;
}