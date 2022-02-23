#!/bin/sh

set -e
set -x

cmake   -DCMAKE_TOOLCHAIN_FILE=/src/build-tools/pi4/pi4.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_AUDIOENGINE=On \
        -DBUILD_PLAYGROUND=On \
        -DIMPORT_WEB=/web/web.tar.gz \
        -S /src \
        -B /build

make -C /build -j12 
sudo make -C /build install
cd /build && sudo cpack
