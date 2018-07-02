auto_sources(BASE_HPP "*.hpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_CPP "*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
set(APP_NAME "db_proxy_server")
add_executable(${APP_NAME} ${BASE_HPP} ${BASE_CPP})
set_target_properties(${APP_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${BinOutPutDir}
        RUNTIME_OUTPUT_DIRECTORY ${BinOutPutDir}
        LIBRARY_OUTPUT_DIRECTORY ${LibOutPutDir})