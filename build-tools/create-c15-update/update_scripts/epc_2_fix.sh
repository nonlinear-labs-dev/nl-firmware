#!/bin/sh
set -x

# version :     1.0
#
# ----------- EPC2 (NUC10i3 & NUC11i3) Fixes  ---------
# - prevent renaming network deivces
# - add igc module to initcpio
#
# Procedure: will be copied to the ePC und /tmp after update and executed

epc_fix() {
    if ! cat /etc/default/grub | grep "net.ifnames=0 biosdevname=0" > /dev/null; then
        if ! (mkdir -p /mnt/sda2 \
            && mount /dev/sda2 /mnt/sda2 \
            && mount /dev/sda1 /mnt/sda2/boot \
            && arch-chroot /mnt/sda2 /bin/bash -c "sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT=\"quiet net.ifnames=0 biosdevname=0 ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2\"/g' /etc/default/grub" \
            && arch-chroot /mnt/sda2 /bin/bash -c "grub-mkconfig -o /boot/grub/grub.cfg" \
            && umount /mnt/sda2/boot \
            && umount /mnt/sda2); then
               printf "E48 ePC2 update: setting Kernel Params failed" >> /tmp/fix_error.log && ((fix_errors++))
        fi
    fi

    if ! cat /etc/mkinitcpio.conf | grep "MODULES=\"e1000e igc\"" > /dev/null; then
        if ! (mkdir -p /mnt/sda2 \
            && mount /dev/sda2 /mnt/sda2 \
            && mount /dev/sda1 /mnt/sda2/boot \
            && arch-chroot /mnt/sda2 /bin/bash -c "sed -i 's/^MODULES=.*$/MODULES=\"e1000e igc\"/g' /etc/mkinitcpio.conf" \
            && arch-chroot /mnt/sda2 /bin/bash -c "mkinitcpio -p linux-rt" \
            && umount /mnt/sda2/boot \
            && umount /mnt/sda2); then
                printf "E48 ePC2 update: adding ethernet module igc" >> /tmp/fix_error.log && ((fix_errors++))
        fi
    fi
}


main() {
    epc_fix
    return $?
}

main
