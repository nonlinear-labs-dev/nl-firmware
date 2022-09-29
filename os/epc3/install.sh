post_install() {
    sed -i 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=1/' /etc/default/grub
    sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT="quiet net.ifnames=0 biosdevname=0 ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2"/' /etc/default/grub
    sed -i 's/^HOOKS=.*$/HOOKS=\"base udev oroot block filesystems autodetect modconf keyboard net nlhook\"/' /etc/mkinitcpio.conf
    sed -i 's/^BINARIES=.*$/BINARIES=\"tar rsync gzip lsblk udevadm\"/' /etc/mkinitcpio.conf
    sed -i 's/^MODULES=.*$/MODULES=\"e1000e igc\"/' /etc/mkinitcpio.conf
    sed -i "s/#governor=.*$/governor='performance'/" /etc/default/cpupower
    
    echo "GRUB_DISABLE_OS_PROBER=false" >> /etc/default/grub

    echo "root@192.168.10.11:/mnt/usb-stick  /mnt/usb-stick  fuse.sshfs  sshfs_sync,direct_io,cache=no,reconnect,defaults,_netdev,ServerAliveInterval=2,ServerAliveCountMax=3,StrictHostKeyChecking=off  0  0" >> /etc/fstab

    ssh-keygen -A
    systemctl enable cpupower
    systemctl enable sshd
    useradd -m sscl
    echo 'sscl:sscl' | chpasswd
    echo 'sscl ALL=(ALL) NOPASSWD:ALL' >> /etc/sudoers
    
    genfstab -U / >> /etc/fstab
    
    MKINITSCRIPT=/usr/share/libalpm/scripts/mkinitcpio-install
    echo "grub-install --target=x86_64-efi --efi-directory=/boot --bootloader-id=arch_grub --recheck" >> $MKINITSCRIPT
    echo "grub-mkconfig -o /boot/grub/grub.cfg" >> $MKINITSCRIPT
    echo "mkdir -p /boot/EFI/BOOT && cp /boot/EFI/arch_grub/grubx64.efi /boot/EFI/BOOT/BOOTX64.EFI" >> $MKINITSCRIPT
}
