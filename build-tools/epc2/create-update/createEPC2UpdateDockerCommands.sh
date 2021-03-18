set -e

BUILD_SWITCHES="-DBUILD_EPC_SCRIPTS=On -DBUILD_AUDIOENGINE=On -DBUILD_PLAYGROUND=On -DBUILD_ONLINEHELP=On -DBUILD_WEBUI=On"

setup_overlay() {
    mkdir -p /run/shm
    mount -o loop /in/rootfs.ext4 /mnt
    mkdir -p /mnt/boot
    mount -o loop /in/bootfs.fat /mnt/boot/
    
    mkdir -p /overlay-scratch
    mkdir -p /overlay-workdir
    mkdir -p /overlay-fs
    
    fuse-overlayfs -o lowerdir=/mnt -o upperdir=/overlay-scratch -o workdir=/overlay-workdir /overlay-fs || error "fuse-overlay failed."
}

install_packages() {
    pacman --noconfirm -S $UPDATE_PACKAGES
    pacstrap -c /overlay-fs $UPDATE_PACKAGES
}

setup_wifi() {
    (
    cat <<-ENDOFHERE
        [connection]
        id=C15
        uuid=61679179-6804-4197-b476-eacad1d492e4
        type=wifi
        interface-name=wlp0s20f3
        permissions=
    
        [wifi]
        band=bg
        channel=7
        mac-address-blacklist=
        mode=ap
        ssid=Unit-C15-99999

        [wifi-security]
        key-mgmt=wpa-psk
        pairwise=ccmp;
        proto=rsn;  
        psk=88888888

        [ipv4]
        address1=192.168.100.101/24,192.168.100.1
        dns-search=
        method=shared

        [ipv6]
        addr-gen-mode=stable-privacy
        dns-search=
        method=auto

        [proxy]
    ENDOFHERE
    ) > /overlay-fs/etc/NetworkManager/system-connections/C15.nmconnection
    
    arch-chroot /mnt /bin/bash -c "systemctl enable NetworkManager"    
    arch-chroot /mnt /bin/bash -c "systemctl start NetworkManager"
}

build_binaries() {
    DESTDIR=/overlay-fs cmake -DCMAKE_BUILD_TYPE=Release ${BUILD_SWITCHES} -S /source -B /build
    make -j8 -C /build
    DESTDIR=/overlay-fs make -C /build install 
}

create_update() {
    mkdir -p /update
    tar -C /overlay-scratch --exclude=.wh..wh..opq -czf /update/NonLinuxOverlay.tar.gz .
    touch /update/$(sha256sum /update/NonLinuxOverlay.tar.gz | grep -o "^[^ ]*").sign
    tar -cf /out/update.tar ./update
}

setup_overlay
install_packages
setup_wifi
build_binaries
create_update
