#!/bin/sh

# version :     1.0
#
# ----------- fix CPU Bindings and Overlay Order ---------
#
# will be copied to the ePC und /tmp after update and executed


epc_fix() {
    touch /tmp/fix_error.log
    fix_errors=0

    if mount | grep "/lroot:/nloverlay/os-overlay"; then
        if ! (mkdir /tmp/sda2 \
            && mount /dev/sda2 /tmp/sda2 \
            && sed -i 's@/lroot:/nloverlay/os-overlay@/nloverlay/os-overlay:/lroot@' /tmp/sda2/lib/initcpio/hooks/oroot \
            && umount /tmp/sda2 \
            && mkinitcpio -p linux \
            && mkinitcpio -p linux-rt); then
               printf "E48 ePC update: fixing Overlay order failed" >> /tmp/fix_error.log && ((fix_errors++))
        fi
    fi

    if ! cat /etc/default/grub | grep "isolcpus" > /dev/null; then
        if ! cat /etc/default/grub | grep "mitigations" > /dev/null; then
            if ! (mkdir -p /mnt/sda2 \
                && mount /dev/sda2 /mnt/sda2 \
                && mount /dev/sda1 /mnt/sda2/boot \
                && arch-chroot /mnt/sda2 /bin/bash -c "sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT=\\\"quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2\\\"/g' /etc/default/grub" \
                && arch-chroot /mnt/sda2 /bin/bash -c "grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck" \
                && arch-chroot /mnt/sda2 /bin/bash -c "grub-mkconfig -o /boot/grub/grub.cfg" \
                && arch-chroot /mnt/sda2 /bin/bash -c "mkdir -p /boot/EFI/BOOT" \
                && arch-chroot /mnt/sda2 /bin/bash -c "cp /boot/EFI/arch_grub/grubx64.efi /boot/EFI/BOOT/BOOTX64.EFI" \
                && umount /mnt/sda2/boot \
                && umount /mnt/sda2); then
                   printf "E48 ePC update: setting Kernel Params failed" >> /tmp/fix_error.log && ((fix_errors++))
            fi
        fi
    fi

    if ! fsck /dev/sda4 -n; then
        umount /dev/sda4
        if ! mkfs.ext4 /dev/sda4; then
            printf "E48 ePC update: fixing sda4 failed" >> /tmp/fix_error.log && ((fix_errors++))
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

