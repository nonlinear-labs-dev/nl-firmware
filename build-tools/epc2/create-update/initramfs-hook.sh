#!/usr/bin/ash

# stop boot cycle:
rm -f /nloverlay/os-overlay/usr/lib/initcpio/hooks/nlhook_v2

# unpack new kernel:
mkdir -p /kernel-update
tar -C /kernel-update -x -f /nloverlay/os-overlay/kernel-update.tar 

# prepare kexec:
/nloverlay/os-overlay/usr/bin/kexec -l /kernel-update/boot/vmlinuz-linux-rt --initrd=/kernel-update/boot/initramfs-linux-rt.img --reuse-cmdline

# boot into new kernel:
/nloverlay/os-overlay/usr/bin/kexec -e