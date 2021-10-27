#!/bin/sh

set -e

mkdir -p /bindir/build
cd /bindir/build

cmake -DTARGET_PLATFORM=epc2 -DCMAKE_BUILD_TYPE=Release -DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_WEB=Off /srcdir && make -j8
