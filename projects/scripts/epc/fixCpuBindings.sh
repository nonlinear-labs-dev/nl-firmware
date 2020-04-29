#!/bin/sh

if cat /etc/default/grub | grep "isolcpus"; then
    echo "CPU bindings are already set up."
    exit 0
else
    echo "CPU bindings are not set up, will do ..."
    mkdir -p /mnt/sda2
    mount /dev/sda2 /mnt/sda2
    mount /dev/sda1 /mnt/sda2/boot
    arch-chroot /mnt/sda2 /bin/bash -c "sed -i s/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT=\"quiet ip=192.168.10.10:::::eth0:none isolcpus=0,2\" /etc/default/grub"
    arch-chroot /mnt/sda2 /bin/bash -c "grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck"
    arch-chroot /mnt/sda2 /bin/bash -c "grub-mkconfig -o /boot/grub/grub.cfg"
    arch-chroot /mnt/sda2 /bin/bash -c "mkdir -p /boot/EFI/BOOT"
    arch-chroot /mnt/sda2 /bin/bash -c "cp /boot/EFI/arch_grub/grubx64.efi /boot/EFI/BOOT/BOOTX64.EFI"
    umount /mnt/sda2/boot
    umount /mnt/sda2
    reboot
fi
