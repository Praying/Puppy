auto_sources(BASE_HPP "*.hpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_CPP "*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_PROTO_CPP "*.cc" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_PROTO_HPP "*.h" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")

set(APP_NAME "base")
add_library(${APP_NAME} STATIC ${BASE_HPP} ${BASE_CPP} ${BASE_PROTO_HPP} ${BASE_PROTO_CPP})
if(CMAKE_SYSTEM_NAME MATCHES "Linux")
    set(LINUX_DEPS pthread)
else()
    set(LINUX_DEPS )
endif()

target_link_libraries(${APP_NAME} protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite ${LINUX_DEPS} ${Boost_LIBRARIES} glog::glog  ${CURL_LIBRARIES} z  jsoncpp_lib_static OpenSSL::SSL OpenSSL::Crypto )
