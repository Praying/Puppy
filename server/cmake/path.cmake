#可执行文件 输出路径
set(SolutionDir ${PROJECT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(OutPutDir ${PROJECT_SOURCE_DIR}/bin/debug)
else()
    set(OutPutDir ${PROJECT_SOURCE_DIR}/bin/release)
endif()
