#!/bin/sh

set -e
set -x

install_package() {
    name="$1"
    pacstrap -c /rootfs $name=$version && return 0
    
    for ENDING in "pkg.tar.xz" "pkg.tar.zst"; do
        for PLATFORM in "x86_64" "any"; do
            fileName="$name-$version-$PLATFORM.$ENDING"
            if pacstrap -U /rootfs /downloads/epc2/packages/$fileName; then
                return 0
            fi
        done
    done
        
    echo "Could not install package $name in version $version."
    return 1
}

install_packages() {
    while read package; do
        name=$(echo $package | cut -f1 -d " ")
        version=$(echo $package | cut -f2 -d " ")
        echo "install_packages: $package  => $name" 
        install_package $name $version || return 1
    done < /epc2-binary-dir/.epc2-base-os-final-packages

    return 0
}

create_package_database() {
    echo "[nonlinux]" > /etc/pacman.conf
    echo "Server = file:///downloads/epc2/packages/" >> /etc/pacman.conf
    pacman -Sy > /dev/null
}

perform_tweaks() {
    cp /current-source-dir/install/nlhook /rootfs/lib/initcpio/install/nlhook
    cp /current-source-dir/install/oroot /rootfs/lib/initcpio/install/oroot
    cp /current-source-dir/hook/nlhook /rootfs/lib/initcpio/hooks/nlhook
    cp /current-source-dir/hook/oroot /rootfs/lib/initcpio/hooks/oroot

    sed -i 's/GRUB_TIMEOUT=5/GRUB_TIMEOUT=1/' /rootfs/etc/default/grub
    sed -i 's/GRUB_CMDLINE_LINUX_DEFAULT.*$/GRUB_CMDLINE_LINUX_DEFAULT="quiet ip=192.168.10.10:::::eth0:none mitigations=off isolcpus=0,2"/' /rootfs/etc/default/grub

    sed -i 's/^HOOKS=.*$/HOOKS=\"base udev oroot block filesystems autodetect modconf keyboard net nlhook\"/' /rootfs/etc/mkinitcpio.conf
    sed -i 's/^BINARIES=.*$/BINARIES=\"tar rsync gzip lsblk udevadm\"/' /rootfs/etc/mkinitcpio.conf
    sed -i 's/^MODULES=.*$/MODULES=\"e1000e\"/' /rootfs/etc/mkinitcpio.conf
    sed -i "s/#governor=.*$/governor='performance'/" /rootfs/etc/default/cpupower

    echo "root@192.168.10.11:/mnt/usb-stick  /mnt/usb-stick  fuse.sshfs  sshfs_sync,direct_io,cache=no,reconnect,defaults,_netdev,ServerAliveInterval=2,ServerAliveCountMax=3,StrictHostKeyChecking=off  0  0" >> /rootfs/etc/fstab

    arch-chroot /rootfs /bin/bash -c "systemctl enable cpupower"
    arch-chroot /rootfs /bin/bash -c "systemctl enable sshd"
    arch-chroot /rootfs bash -c "useradd -m sscl"
    arch-chroot /rootfs bash -c "echo 'sscl:sscl' | chpasswd"
    echo "sscl   ALL=(ALL) NOPASSWD:ALL" >> /rootfs/etc/sudoers
}

create_mountpoint() {
    mkdir /rootfs
    yes | truncate -s 8G /current-binary-dir/rootfs.ext4
    yes | truncate -s 512M /current-binary-dir/bootfs.fat
    yes | mkfs.ext4 /current-binary-dir/rootfs.ext4
    yes | mkfs.fat /current-binary-dir/bootfs.fat
    chown $USER_ID:$GROUP_ID /current-binary-dir/rootfs.ext4
    chown $USER_ID:$GROUP_ID /current-binary-dir/bootfs.fat
    mount -o loop /current-binary-dir/rootfs.ext4 /rootfs
    mkdir /rootfs/boot
    mount -o loop /current-binary-dir/bootfs.fat /rootfs/boot/
    mkdir -p /run/shm
}


finalize() {
  arch-chroot /rootfs mkinitcpio -p linux-rt
}

create_mountpoint
create_package_database
install_packages /rootfs || exit 1
perform_tweaks
finalize