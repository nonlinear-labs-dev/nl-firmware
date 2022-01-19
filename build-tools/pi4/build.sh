#!/bin/sh

set -e

cmake   -DCMAKE_PREFIX_PATH=/build/NL-1.0/usr/local \
        -DCMAKE_INSTALL_PREFIX=/build/NL-1.0/usr/local \
        -DCMAKE_TOOLCHAIN_FILE=/src/build-tools/pi4/pi4.cmake \
        -DCMAKE_BUILD_TYPE=Release \
        -DBUILD_AUDIOENGINE=On \
        -DBUILD_PLAYGROUND=On \
        -S /src \
        -B /build

make -C /build -j12 install
find /build/NL-1.0 -type f -name *.service | xargs -i sed -i 's|/build/NL-1.0||g' {}
mkdir -p /build/NL-1.0/DEBIAN
cp /DEBIAN/* /build/NL-1.0/DEBIAN/
chmod 775 /build/NL-1.0/DEBIAN/postinst
chmod 775 /build/NL-1.0/DEBIAN/postrm
dpkg-deb -b /build/NL-1.0
