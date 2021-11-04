#!/bin/sh

set -e

mkdir -p /bindir/build
cd /bindir/build

cmake _DCMAKE_INSTALL_DIR=/usr/local/C15 -DTARGET_PLATFORM=epc2 -DCMAKE_BUILD_TYPE=Release -DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_WEB=Off /srcdir
make -j8
make install 

mkdir -p /usr/local/C15/web
tar -xzf /web/web.tar.gz -C /usr/local/C15/web

set -x
ls /usr/local/

ls /usr/local/C15


ls /usr/local/C15/playground

ls /usr/local/C15/web
