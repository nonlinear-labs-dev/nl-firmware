#!/bin/sh

copy_running_os() {
    echo "Copying running os..."
    if tar -C /internal/os --exclude=./build/CMakeFiles -czf /update-scratch/update/NonLinuxOverlay.tar.gz .; then
        echo "Copying running os done."
        return 0
    fi
    echo "Copying running os failed."
    return 1
}

calc_checksum() {
    echo "Calc checksum..."
    if (cd /update-scratch/update/ && touch $(sha256sum ./NonLinuxOverlay.tar.gz | grep -o "^[^ ]*").sign); then
        echo "Calc checksum done."
        return 0
    fi
    echo "Calc checksum failed."
    return 1
}

create_update_tar() {
    echo "Create update.tar..."
    if (cd /update-scratch/ && tar -cf /update.tar ./update); then
        echo "Create update.tar done."
        return 0
    fi
    echo "Create update.tar failed."
    return 1
}

create_update() {
    echo "Creating update..."
    
    mkdir -p /update-scratch/update

    if copy_running_os && calc_checksum && create_update_tar; then
        echo "Created update done."
        return 0
    fi

    echo "Creating update failed."
    return 1
}

deploy_update() {
    mv /update.tar /workdir/update.tar
}

setup_build_overlay() {
    fuse-overlayfs -o lowerdir=/workdir/squashfs-root -o upperdir=/workdir/overlay-scratch -o workdir=/workdir/overlay-workdir /workdir/overlay-fs
    mkdir /workdir/overlay-fs/sources
    mkdir /workdir/overlay-fs/build
    mkdir /workdir/epc-nl-build
    mount --bind /sources /workdir/overlay-fs/sources
    mount --bind /workdir/epc-nl-build /workdir/overlay-fs/build
}

build_c15() {
    /workdir/overlay-fs/bin/arch-chroot /workdir/overlay-fs /bin/bash -c "\
        cd /build
        cmake /sources -DCMAKE_BUILD_TYPE=Release -DBUILD_AUDIOENGINE=On -DBUILD_BBBB=Off -DBUILD_PLAYGROUND=On -DBUILD_ONLINEHELP=On -DBUILD_TEXT2SOLED=Off -DBUILD_TESTING=Off
        make -j8"
}

setup_install_overlay() {
    mkdir -p /internal
    cp -ra /workdir/overlay-fs /internal/built
    mkdir -p /internal/os /internal/ow /internal/epc-update-partition
    fuse-overlayfs -o lowerdir=/internal/built -o upperdir=/internal/os -o workdir=/internal/ow /internal/epc-update-partition
}

install_c15() {
    /internal/epc-update-partition/bin/arch-chroot /internal/epc-update-partition /bin/bash -c "\
        cd /build
        make install
        systemctl enable /usr/local/lib/systemd/system/playground.service
        systemctl enable /usr/local/lib/systemd/system/audio-engine.service"
}

setup_build_overlay
build_c15
setup_install_overlay
install_c15
create_update
deploy_update

