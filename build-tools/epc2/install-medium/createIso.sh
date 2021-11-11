#!/bin/sh

set -x
set -e

pacman --noconfirm -Syy
pacman --noconfirm -S archiso

cp -r /usr/share/archiso/configs/baseline/ /archlive
echo dosfstools >> /archlive/packages.x86_64
echo arch-install-scripts >> /archlive/packages.x86_64
echo nano >> /archlive/packages.x86_64
cp /out/rootfs/bootfs.fat /archlive/airootfs/
cp /out/rootfs/rootfs.ext4 /archlive/airootfs/
cp /src/runme.sh /archlive/airootfs/
cp /src/sda.sfdisk /archlive/airootfs/
chmod +x /archlive/airootfs/runme.sh
gzip /archlive/airootfs/bootfs.fat
gzip /archlive/airootfs/rootfs.ext4
mkdir -p /workdir
ISONAME=$(mkarchiso -v -w /work_dir -o /out /archlive | grep -o "/out/archlinux-baseline-.*-x86_64.iso")
mv $ISONAME /out/install-medium/epc2os.iso
