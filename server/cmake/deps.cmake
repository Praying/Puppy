find_package(glog CONFIG REQUIRED)
#find_package(double-conversion CONFIG REQUIRED)
#find_package(folly CONFIG REQUIRED)
#find_package(fmt CONFIG REQUIRED)
#find_package(spdlog CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(Boost COMPONENTS system filesystem program_options)
include_directories(${FOLLY_INCLUDE_DIR})
