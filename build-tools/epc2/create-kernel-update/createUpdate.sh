#!/bin/sh

rm /boot/initramfs-linux-rt-fallback.img
tar -c -f /bindir/update.tar /boot
