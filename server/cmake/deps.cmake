find_package(glog CONFIG REQUIRED)
find_package(folly CONFIG REQUIRED)
find_package(TBB CONFIG REQUIRED)
find_package(fmt CONFIG REQUIRED)
find_package(log4cplus CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
include_directories(${FOLLY_INCLUDE_DIR})