#!/bin/bash

cd /opt/opendlv-sources

if [ "$1" == "FRESH" ]; then
    rm -fr build.from.docker
    mkdir build.from.docker
    cd build.from.docker
    mkdir build
    mkdir install

    cd build
    cmake -D CMAKE_INSTALL_PREFIX=../../docker/fh16/opendlv ../..
    make -j3
    cd ..

    mkdir build.system
    cd build.system
    cmake -DEIGEN3_INCLUDE_DIR=../../thirdparty/eigen3 -DKALMAN_INCLUDE_DIR=../../thirdparty/kalman/include -DTINYCNN_INCLUDE_DIR=../../thirdparty/tiny-cnn -DCMAKE_INSTALL_PREFIX=../../docker/fh16/opendlv ../../code/system
    make -j3
    make install
    cd ..
fi

if [ "$1" == "INCREMENTAL" ]; then
    if [ -d build.from.docker -a -d build.from.docker/build -a -d build.from.docker/../docker/fh16/opendlv -a -d build.from.docker/build.system ]; then
        cd build.from.docker
        cd build.system
        make -j3
        make install
    else
        exit 1
    fi
fi

