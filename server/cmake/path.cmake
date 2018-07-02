#可执行文件 输出路径
set(SolutionDir ${PROJECT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BinOutPutDir ${PROJECT_SOURCE_DIR}/bin/debug)
    set(LibOutPutDir ${PROJECT_SOURCE_DIR}/lib/debug)
else()
    set(BinOutPutDir ${PROJECT_SOURCE_DIR}/bin/release)
    set(LibOutPutDir ${PROJECT_SOURCE_DIR}/lib/release)
endif()

#set(EXECUTABLE_OUTPUT_PATH ${BinOutPutDir})
#set(LIBRARY_OUTPUT_PATH ${LibOutPutDir})