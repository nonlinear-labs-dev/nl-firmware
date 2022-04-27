#!/bin/sh

set -x
set -e

ROOT_FS="$1"

if cat /proc/cpuinfo | grep "model name" | head -n1 | grep "i3-5010" > /dev/null; then
  echo "5i3  => epc1"
elif cat /proc/cpuinfo | grep "model name" | head -n1 | grep "i3-7100" > /dev/null; then
  echo "7i3  => epc1"
elif cat /proc/cpuinfo | grep "model name" | head -n1 | grep "i3-1011" > /dev/null; then
  echo "10i3 => epc2"
  ENABLE_EPC_WIFI=1
elif cat /proc/cpuinfo | grep "model name" | head -n1 | grep "i3-1115" > /dev/null; then
  echo "11i3 => epc3"
  FIX_BUFFER_UNDERRUNS=1
  ENABLE_EPC_WIFI=1
else
  echo "Unsupported platform: "
  cat /proc/cpuinfo | grep "model name"
  exit 1
fi

if [ $FIX_BUFFER_UNDERRUNS == 1 ]; then
  echo "options snd_hda_intel bdl_pos_adj=1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1" > $ROOT_FS/etc/modprobe.d/snd_hda_intel.conf
  chmod 600 $ROOT_FS/etc/modprobe.d/snd_hda_intel.conf
fi

if [ $ENABLE_EPC_WIFI == 1 ]; then
  cp /nloverlay/update-scratch/update/wpa_supplicant.conf $ROOT_FS/etc/wpa_supplicant/wpa_supplicant.conf
  chmod 600 $ROOT_FS/etc/wpa_supplicant/wpa_supplicant.conf
  cp /nloverlay/update-scratch/update/C15.nmconnection $ROOT_FS/etc/NetworkManager/system-connections/C15.nmconnection
  chmod 600 $ROOT_FS/etc/NetworkManager/system-connections/C15.nmconnection
fi
