#!/bin/sh

if [ -e /mnt/usb-stick/nonlinear-c15-update.tar ]; then
    if rm -rf /update \
        && mkdir -p /update \
        && cp /mnt/usb-stick/nonlinear-c15-update.tar /update \
        && cd /update \
        && tar xf nonlinear-c15-update.tar; then
            mv /mnt/usb-stick/nonlinear-c15-update.tar /mnt/usb-stick/nonlinear-c15-update.tar-copied
            if ! ls -l /update/BBB/ | grep bbb_update_1811a.sh \
                && ! ls -l /update/BBB/ | grep bbb_update_1905a.sh \
                && ! ls -l /update/BBB/ | grep playground-RELEASE-2019-05-1.5-706569a; then
                chmod +x /update/run.sh
                /bin/sh /update/run.sh
            fi
    fi
fi
