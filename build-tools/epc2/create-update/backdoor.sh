#! /bin/sh

set -x

uname -a | grep "@EPC2_UPDATE_KERNEL@" && exit 1
echo "Kernel has to be updated."
checksum=$(sha256sum ./NonLinuxOverlay.tar.gz | cut -d " " -f 1)
checksumFile=./${checksum}.sign
[ -f ${checksumFile} ] || exit 1
echo "Checksum verified."
rm -rf /nloverlay/os-overlay/*
echo "overlayfs cleaned."
tar -C /nloverlay/os-overlay -xzf ./NonLinuxOverlay.tar.gz || exit 1
echo "Replaced overlayfs."
rm -rf ./tmp
mkdir ./tmp
tar -x -C ./tmp -f /nloverlay/os-overlay/kernel-update.tar
echo "Kernel unpacked"
/nloverlay/os-overlay/usr/bin/kexec -l ./tmp/boot/vmlinuz-linux-rt --initrd=./tmp/boot/initramfs-linux-rt.img --reuse-cmdline
echo "Kernel exec..."
/nloverlay/os-overlay/usr/bin/kexec -e

exit 0
