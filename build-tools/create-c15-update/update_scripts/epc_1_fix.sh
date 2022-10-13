#!/bin/sh

# version :     1.0
#
# ----------- EPC1 (NUC5i3 & NUC7i3) Fixes  ---------
# - CPU Bindings
# - Overlay Order
#
# Procedure: will be copied to the ePC und /tmp after update and executed

set -x

fix_overlay_order() {
  if mount | grep "/lroot:/nloverlay/os-overlay"; then
    sed -i 's@/lroot:/nloverlay/os-overlay@/nloverlay/os-overlay:/lroot@' /tmp/sda2/lib/initcpio/hooks/oroot
    arch-chroot /tmp/sda2 mkinitcpio -p linux-rt \
      || printf "E48 ePC1 fix: fixing Overlay order failed" >> /tmp/fix_error.log && ((fix_errors++))
  fi
}

fix_kernel_cmdline() {
  if ! cat /proc/cmdline | grep "isolcpus" | grep "mitigations" > /dev/null; then
    arch-chroot /tmp/sda2 /bin/bash -c "sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT=\\\"quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2\\\"/g' /etc/default/grub"
    arch-chroot /tmp/sda2 /bin/bash -c "grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck" \
      && arch-chroot /tmp/sda2 /bin/bash -c "grub-mkconfig -o /boot/grub/grub.cfg" \
      && arch-chroot /tmp/sda2 /bin/bash -c "mkdir -p /boot/EFI/BOOT" \
      && arch-chroot /tmp/sda2 /bin/bash -c "cp /boot/EFI/arch_grub/grubx64.efi /boot/EFI/BOOT/BOOTX64.EFI" \
      || printf "E48 ePC1 fix: setting Kernel Params failed" >> /tmp/fix_error.log && ((fix_errors++))
  fi
}

epc_fix() {
    touch /tmp/fix_error.log
    fix_errors=0
    
    mkdir -p /tmp/sda2
    mount /dev/sda2 /tmp/sda2
    mount /dev/sda1 /tmp/sda2/boot

    fix_overlay_order
    fix_kernel_cmdline
    
    umount /tmp/sda2/boot
    umount /tmp/sda2

    if ! fsck /dev/sda4 -n; then
        umount /dev/sda4
        if ! mkfs.ext4 /dev/sda4; then
            printf "E48 ePC1 update: fixing sda4 failed" >> /tmp/fix_error.log && ((fix_errors++))
        fi
    fi

    if [ $fix_errors -gt 0 ]; then
         return 48
    fi
    return 0
}

main() {
    epc_fix
    return $?
}

main

