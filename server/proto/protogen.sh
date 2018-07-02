#!/bin/sh
protodir=Protos
protogendir=ProtosGen
if [ "$(uname)" == "Darwin" ]; then
    # Do something under Mac OS X platform
    prototargetdir=$protogendir/Mac
elif [ "$(expr substr $(uname -s) 1 5)" == "Linux" ]; then
    # Do something under GNU/Linux platform
    prototargetdir=$protogendir/Linux
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW32_NT" ]; then
    # Do something under 32 bits Windows NT platform
    prototargetdir=$protogendir/Win
elif [ "$(expr substr $(uname -s) 1 10)" == "MINGW64_NT" ]; then
    # Do something under 64 bits Windows NT platform
    prototargetdir=$protogendir/Win
fi
if [ ! -d "$prototargetdir" ]; then 
	mkdir "$prototargetdir"
fi 
protoc -I=$protodir --cpp_out=$prototargetdir --python_out=$prototargetdir  $protodir/*.proto
cp $prototargetdir/* $protogendir
