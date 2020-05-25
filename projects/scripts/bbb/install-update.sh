#!/bin/sh

if [ -e /mnt/usb-stick/nonlinear-c15-update.tar ]; then
    rm -rf /update \
    && mkdir -p /update \
    && cp /mnt/usb-stick/nonlinear-c15-update.tar /update \
    && cd /update \
    && tar xf nonlinear-c15-update.tar

    if [ $? -eq 0 ]; then
        mv /mnt/usb-stick/nonlinear-c15-update.tar /mnt/usb-stick/nonlinear-c15-update.tar-copied
        chmod +x /update/run.sh
        /bin/sh /update/run.sh
    fi
fi
