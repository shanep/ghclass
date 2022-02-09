#!/bin/bash
if [ ! -d ./build ]
then
    mkdir build
    cd build
    cmake -DCMAKE_BUILD_TYPE=Debug ..
    cmake --build .
else 
    echo "Build directory is already present. Run clean.sh first!"
fi