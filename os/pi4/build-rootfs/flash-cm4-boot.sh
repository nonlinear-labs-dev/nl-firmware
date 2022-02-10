#!/bin/bash

set -e

[ -z $PI4_DEVICE ] && (echo "PI4_DEVICE environment var has to be set, e.g. PI4_DEVICE=sde" && exit 1)

RO=$(lsblk -ro KNAME,RO,RM | grep $PI4_DEVICE | head -n1 | cut -f 2 -d ' ' | xargs)
RM=$(lsblk -ro KNAME,RO,RM | grep $PI4_DEVICE | head -n1 | cut -f 3 -d ' ' | xargs)

[ "$RO" == "0" ] || (echo "device $PI4_DEVICE is read only" && exit 1)
[ "$RM" == "1" ] || (echo "device $PI4_DEVICE is not removeable" && exit 1)

OF="/dev/$PI4_DEVICE"1

dd if=/current-binary-dir/raspios.img count=4096 bs=65536 skip=64 of=$OF status=progress
