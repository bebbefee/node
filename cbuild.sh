#!/bin/bash

# publish dir
BUILD_DIR="build"
BIN_DIR="bin"

# version
VERSION="Debug"

if [ "$1" == "clean" ];then
   rm -rf $BUILD_DIR
   exit
elif [ "$1" == "debug" ];then
    BIN_DIR+="_debug"
    VERSION="Debug"
elif [ "$1" == "release" ];then
    BIN_DIR+="_release"
    VERSION="Release"
else
    echo 'params is debug or release'
    exit
fi

# begin
echo -e '==> build start ...'

# prepare dir
echo -e '==> prepare output directory ...'
mkdir -p $BUILD_DIR
mkdir -p $BIN_DIR

# make it
echo -e '\n==> make -j8 ...\n'

if [ "$2" == "rebuild" ];then
   rm -f CMakeCache.txt
   rm -rf CMakeFiles
   rm -f Makefile
   rm -rf ./temp_$1
fi

cd $BUILD_DIR

cmake -DCMAKE_BUILD_TYPE=$VERSION ../

time make

cd ..
echo -e '\n==> cbuild finish !'
