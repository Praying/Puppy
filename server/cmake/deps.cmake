find_package(glog CONFIG REQUIRED)
#find_package(double-conversion CONFIG REQUIRED)
#find_package(folly CONFIG REQUIRED)
#find_package(fmt CONFIG REQUIRED)
#find_package(spdlog CONFIG REQUIRED)
find_package(CURL REQUIRED)
find_package(jsoncpp CONFIG REQUIRED)
find_package(protobuf CONFIG REQUIRED)
find_package(Boost COMPONENTS system filesystem program_options)
find_package(OpenSSL REQUIRED)
include_directories(${FOLLY_INCLUDE_DIR})
