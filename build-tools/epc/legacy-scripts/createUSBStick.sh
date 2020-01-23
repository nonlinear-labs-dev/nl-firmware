#!/bin/bash

ISO_FILE=$1
STICK=$2
TIMESTAMP=`date +%m-%d-%Y-%H:%M:%S`
MOUNTPOINT=$HOME/create-USB-stick-$TIMESTAMP

exit_script() {
    sudo umount $MOUNTPOINT/src
    sudo umount $MOUNTPOINT/tgt
    sudo rm -rf $MOUNTPOINT

    if [ -n "$LOOPDEVICE" -a -b "$LOOPDEVICE" ]; then
        echo "losetup... $LOOPDEVICE"
        sudo losetup -d $LOOPDEVICE
    fi
    
    echo $1 
    exit 0
}

check_preconditions() {
    echo $FUNCNAME

    if [ -z $ISO_FILE ]; then
        exit_script "usage: $0 /path/to/NonLinux.iso /dev/sdX"
    fi

    if [ -z $STICK ]; then
        exit_script "usage: $0 /path/to/NonLinux.iso /dev/sdX"
    fi

    if [ ! -f $ISO_FILE ]; then
        exit_script "file $ISO_FILE does not exist."
    fi

    if [ ! -b $STICK ]; then
        exit_script "$STICK seems not to be block device file."
    fi

    if ! lsblk -l -p -o NAME,MOUNTPOINT,RM,TYPE | grep $STICK | grep "1 disk" >> /dev/null; then
        exit_script "$STICK seems not to be a removeable USB storage device."
    fi
}

unmount_stick() {
    for p in $(lsblk -lp -o NAME | grep ${STICK}[0-9]); do
        sudo umount $p
    done
}

partition_stick() {
    echo $FUNCNAME
    unmount_stick
    CMD="label: dos

label-id: 0x16ed9305
device: /dev/sde
unit: sectors

/dev/sde1 : start= 2048, size= 7687424, type=ef, bootable
"

    echo "$CMD" | sudo sfdisk $STICK 
    if ! sudo mkfs.msdos ${STICK}1; then
        exit_script "partitioning stick failed."
    fi
}

mount_iso() {
    echo $FUNCNAME
    mkdir -p $MOUNTPOINT/src
    LOOPDEVICE=$(sudo losetup -f --show -P $ISO_FILE)
    if ! sudo mount ${LOOPDEVICE}p1 $MOUNTPOINT/src; then 
        exit_script "Could not mount the source ISO file."
    fi
}

mount_target() {
    mkdir -p $MOUNTPOINT/tgt
    if ! sudo mount ${STICK}1 $MOUNTPOINT/tgt; then 
        exit_script "Could not mount the USB stick."
    fi
}

copy_fs() {
    echo $FUNCNAME
    if ! sudo cp -a $MOUNTPOINT/src/* $MOUNTPOINT/tgt/; then 
        exit_script "Could not copy the ISO image content onto the stick."
    fi
    sync
}

main() {
    check_preconditions
    partition_stick
    mount_iso
    mount_target
    copy_fs
    exit_script "Success!"
}

main