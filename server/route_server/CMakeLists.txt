auto_sources(BASE_HPP "*.hpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_CPP "*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
set(APP_NAME "route_server")
add_executable(${APP_NAME} ${BASE_HPP} ${BASE_CPP})
target_link_libraries(${APP_NAME})
set_target_properties(${APP_NAME} PROPERTIES
        ARCHIVE_OUTPUT_DIRECTORY ${OutPutDir}
        RUNTIME_OUTPUT_DIRECTORY ${OutPutDir}
        LIBRARY_OUTPUT_DIRECTORY ${OutPutDir})