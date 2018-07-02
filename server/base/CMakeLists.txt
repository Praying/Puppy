auto_sources(BASE_HPP "*.hpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_CPP "*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
set(APP_NAME "base")
add_library(${APP_NAME} ${BASE_HPP} ${BASE_CPP})
#set_target_properties(${APP_NAME} PROPERTIES
#        ARCHIVE_OUTPUT_DIRECTORY ${LibOutPutDir}
#        RUNTIME_OUTPUT_DIRECTORY ${LibOutPutDir}
#        LIBRARY_OUTPUT_DIRECTORY ${LibOutPutDir})
target_link_libraries(${APP_NAME} PRIVATE Folly::folly Folly::folly_deps Folly::follybenchmark  log4cplus::log4cplusU protobuf::libprotoc protobuf::libprotobuf protobuf::libprotobuf-lite)
