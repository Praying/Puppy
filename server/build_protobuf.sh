if [ ! -d "downloads" ]; then 
  mkdir downloads 
fi 
wget --directory-prefix=./downloads/ https://github.com/google/protobuf/releases/download/v3.5.1/protobuf-cpp-3.5.1.tar.gz 
cd downloads
tar xf protobuf-cpp-3.5.1.tar.gz    
cd protobuf-3.5.1
./configure
make -j20 
