#!/bin/sh

set -x


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


fuse-overlayfs -o lowerdir=/workdir/squashfs-root -o upperdir=/workdir/overlay-scratch -o workdir=/workdir/overlay-workdir /workdir/overlay-fs
mkdir /workdir/overlay-fs/sources
mkdir /workdir/overlay-fs/build
mkdir /workdir/epc-nl-build
mount --bind /sources /workdir/overlay-fs/sources
mount --bind /workdir/epc-nl-build /workdir/overlay-fs/build

/workdir/overlay-fs/bin/arch-chroot /workdir/overlay-fs /bin/bash -c "\
cd /build
cmake /sources -DBUILD_AUDIOENGINE=On -DBUILD_BBBB=Off -DBUILD_PLAYGROUND=On -DBUILD_ONLINEHELP=On -DBUILD_TEXT2SOLED=Off -DBUILD_TESTING=Off
make -j8
"

mkdir -p /internal

cp -ra /workdir/overlay-fs /internal/built
mkdir -p /internal/os /internal/ow /internal/epc-update-partition
fuse-overlayfs -o lowerdir=/internal/built -o upperdir=/internal/os -o workdir=/internal/ow /internal/epc-update-partition

/internal/epc-update-partition/bin/arch-chroot /internal/epc-update-partition /bin/bash -c "\
cd /build
make install
installPath=`make ServiceFileInstallPath | grep 'SERVICE_INSTALL_PATH' | awk '{split($0,a," "); print a[2]}'`
systemctl enable $installPath/playground.service
systemctl enable $installPath/audio-engine.service
"
echo "make install done"
create_update

mv /update.tar /workdir/update.tar
