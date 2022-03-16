#!/bin/sh

set -x
set -e

cp -r /usr/share/archiso/configs/baseline/ /archlive
echo dosfstools >> /archlive/packages.x86_64
echo arch-install-scripts >> /archlive/packages.x86_64
echo nano >> /archlive/packages.x86_64
cp /out/build-rootfs/bootfs.fat /archlive/airootfs/
cp /out/build-rootfs/rootfs.ext4 /archlive/airootfs/
cp /src/runme.sh /archlive/airootfs/
cp /src/sda.sfdisk /archlive/airootfs/
chmod +x /archlive/airootfs/runme.sh
gzip /archlive/airootfs/bootfs.fat
gzip /archlive/airootfs/rootfs.ext4
mkdir -p /workdir

cat <<- ENDOFHERE > /archlive/pacman.conf
[options]
HoldPkg     = pacman glibc
Architecture = auto
SigLevel    = Required DatabaseOptional
LocalFileSigLevel = Optional

[core]
Include = /etc/pacman.d/mirrorlist
SigLevel = Never

[extra]
Include = /etc/pacman.d/mirrorlist
SigLevel = Never

[community]
Include = /etc/pacman.d/mirrorlist
SigLevel = Never
ENDOFHERE

ISONAME=$(mkarchiso -v -w /work_dir -o /out /archlive | grep -o "/out/archlinux-baseline-.*-x86_64.iso")
mv $ISONAME /out/create-install-medium/epc2os.iso
