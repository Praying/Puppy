# !/bin/bash
if [ -d "build" ]; then 
  sudo rm -rf build 
fi
mkdir build
cd build
cmake -DCMAKE_TOOLCHAIN_FILE=/home/quran/SourceCode/vcpkg/scripts/buildsystems/vcpkg.cmake -DCMAKE_BUILD_TYPE=Debug ..
make -j20
