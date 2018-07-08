- ##### step 1, install vcpkg

- ##### step 2, install third library by vcpkg
    - for Windows
```$shell
./vcpkg install glog:x64-windows
./vcpkg install folly:x64-windows
./vcpkg install tbb:x64-windows
./vcpkg install log4cplus:x64-windows
./vcpkg install asio:x64-windows
./vcpkg install boost:x64-windows
./vcpkg install fmt:x64-windows
./vcpkg install protobuf:x64-windows
```
    - for Linux
```
sudo apt install libpthread-stubs0-dev -y
sudo apt install libevent-dev -y #folly will link it 
./vcpkg install spdlog
./vcpkg install folly
./vcpkg install asio
./vcpkg install boost-property-tree
./vcpkg install protobuf

```