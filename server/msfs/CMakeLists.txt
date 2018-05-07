auto_sources(BASE_HPP "*.hpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
auto_sources(BASE_CPP "*.cpp" "RECURSE" "${CMAKE_CURRENT_SOURCE_DIR}")
set(APP_NAME "msfs")
add_executable(${APP_NAME} ${BASE_HPP} ${BASE_CPP})