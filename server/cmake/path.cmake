#可执行文件 输出路径
set(SolutionDir ${PROJECT_SOURCE_DIR})

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    set(BinOutPutDir ${PROJECT_SOURCE_DIR}/bin/debug)
    set(LibOutPutDir ${PROJECT_SOURCE_DIR}/lib/debug)
else()
    set(BinOutPutDir ${PROJECT_SOURCE_DIR}/bin/release)
    set(LibOutPutDir ${PROJECT_SOURCE_DIR}/lib/release)
endif()

#Executables are always treated as runtime targets
set(EXECUTABLE_OUTPUT_PATH  ${BinOutPutDir})
set(RUNTIME_OUTPUT_DIRECTORY ${BinOutPutDir})
#Static libraries are always treated as archive targets
set(ARCHIVE_OUTPUT_DIRECTORY ${LibOutPutDir})
#Module libraries are always treated as library targets.
#For non-DLL platforms shared libraries are treated as library targets.
#For DLL platforms the DLL part of a shared library is treated as a runtime target and the corresponding import library is treated as an archive target
set(LIBRARY_OUTPUT_PATH ${LibOutPutDir})