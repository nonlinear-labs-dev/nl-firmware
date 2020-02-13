#!/bin/sh

error() {
    echo \"COULD NOT CREATE INSTALL MEDIUM:\"
    echo $1
    exit 1
}

TARGET="$1"
PARTITION="$TARGET"1
CMAKE_CURRENT_BINARY_DIR=$2

[ ! -z "$TARGET" ] || error "You have to specifiy the target device like this: \"EPC_INSTALL_MEDIUM=/dev/sdX make epc-install-medium\""
(lsblk -o PATH,RM | grep "$TARGET " | grep " 1") || error "Sepcified device seems NOT to be removeable"
which fatlabel || error "The binary 'fatlabel' has to be installed on the dev pc"

sudo umount $CMAKE_CURRENT_BINARY_DIR/NonLinux-mnt
sudo umount $CMAKE_CURRENT_BINARY_DIR/InstallMedium-mnt

CMD="label: dos

label-id: 0x16ed9305
device: $TARGET
unit: sectors

$PARTITION : start= 2048, size= 7687424, type=ef, bootable
"

(echo "$CMD" | sudo sfdisk $TARGET) || error "Could not parition target device"
sudo mkfs.msdos $PARTITION || error "Could not mk filesystem on target device"

mkdir -p $CMAKE_CURRENT_BINARY_DIR/NonLinux-mnt
mkdir -p $CMAKE_CURRENT_BINARY_DIR/InstallMedium-mnt
sudo mount -o loop $CMAKE_CURRENT_BINARY_DIR/NonLinuxAP.iso $CMAKE_CURRENT_BINARY_DIR/NonLinux-mnt || error "Could not mount source ISO image"
sudo mount $PARTITION $CMAKE_CURRENT_BINARY_DIR/InstallMedium-mnt || error "Could not mount target device: $TARGET"
sudo cp -a $CMAKE_CURRENT_BINARY_DIR/NonLinux-mnt/* $CMAKE_CURRENT_BINARY_DIR/InstallMedium-mnt || error "Could not copy content to target device"
sudo umount $CMAKE_CURRENT_BINARY_DIR/InstallMedium-mnt || error "Problems unmounting the target device"
sudo fatlabel $PARTITION ARCH_201704 || error "Could not label the partition correctly, stick will most likely not boot"
