#!/bin/sh

set -e

cmake   -DCMAKE_TOOLCHAIN_FILE=/src/build-tools/pi4/pi4.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_AUDIOENGINE=On \
        -DBUILD_PLAYGROUND=On \
        -S /src \
        -B /build

make -C /build -j12 
sudo make -C /build install

cp -r /DEBIAN /build/
chmod 775 /build/DEBIAN/postinst
chmod 775 /build/DEBIAN/postrm
cd /build && sudo cpack
