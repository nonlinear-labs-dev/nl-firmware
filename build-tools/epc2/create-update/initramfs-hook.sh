#!/usr/bin/ash

set -x

# stop boot cycle:
rm -f /nloverlay/os-overlay/usr/lib/initcpio/hooks/nlhook_v2

# unpack new kernel:
mkdir -p /kernel-update
tar -C /kernel-update -x -f /nloverlay/os-overlay/kernel-update.tar 

# prepare kexec:
ROOT=$(blkid /dev/sda2 -o value | head -n1)
/nloverlay/os-overlay/usr/bin/kexec \
    -l /kernel-update/boot/vmlinuz-linux-rt \
    --initrd=/kernel-update/boot/initramfs-linux-rt.img \
    --command-line="BOOT_IMAGE=/vmlinuz-linux-rt root=UUID=$ROOT rw mitigations=off isolcpus=0,2"

# boot into new kernel:
/nloverlay/os-overlay/usr/bin/kexec -e
 