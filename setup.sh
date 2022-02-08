#!/bin/bash
if [ ! -d ./build ]
then
    mkdir build
    cd build
    cmake .. 
else 
    echo "Build directory is already present. Run clean.sh first!"
fi

