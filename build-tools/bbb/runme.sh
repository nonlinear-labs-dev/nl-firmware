#!/bin/sh
set -x

# This function should be run, once the BBB has booted from the SD Card.
# it shall format the eMMC, rsync with the rootfs present on the SD Card,
# deploy the MLO and u-boot.img, tidy up and be done!
# After this the eMMC should be happy booting from eMMC without any SD Card Dependancy!
# boooyah!

EMMC_DEVICE="/dev/mmcblk1"
EMMC_DEVICE_P1="${EMMC_DEVICE}p1"
EMMC_ROOTFS_DIR="/tmp/emmc_rootfs"

is_mounted() {
    if [ "$(cat /proc/mounts | grep ${1})" = "" ]; then
        return 0
    else
        return 1
    fi
}

check_if_mounted_and_unmount() {
    PARTITIONS=$(ls "${EMMC_DEVICE}"?* 2>/dev/null)

    if [ -n "${PARTITIONS}" ]; then
        printf "Checking mountpoints:\n"
        for partition in ${PARTITIONS}; do
            is_mounted ${partition}
            if [ $? -eq 1 ]; then
                printf "${partition}: is mounted. Unmounting...\n"
                umount "${partition}" 2>/dev/null
            else
            printf "${partition}: is not mounted. Ok...\n"
            fi
        done
    fi
}

rewrite_partition() {
    printf "Flushing old partition table...\n"
    dd if=/dev/zero of=${EMMC_DEVICE_P1} bs=1024 count=1024 2>/dev/null 1>/dev/null && sync

    printf "Creating Partition on eMMC... \n"
    if ! echo 'yes' | mkfs.ext4 ${EMMC_DEVICE_P1}; then
        printf "Can not create partition on ${EMMC_DEVICE_P1}. Aborting...\n"
        exit -1
    fi
    sync
}

mount_root() {
    printf "Mounting root partition at ${MOUNT_POINT_ROOT}\n"
    mkdir -p ${MOUNT_POINT_ROOT}
    if ! mount ${EMMC_DEVICE_P1} ${MOUNT_POINT_ROOT}; then
        printf "Can not mount ${EMMC_DEVICE_P1}. Aborting...\n"
        exit -1
    fi
    printf "Mounting root partition at ${MOUNT_POINT_ROOT} done!\n"
}

sync_rootfs() {
    printf "Syncing rootfs to eMMC...\n"
    LD_LIBRARY_PATH="/mmc_install/"

    if ! /mmc_install/rsync -cax --exclude '/mmc_install' --exclude '/tmp' / ${MOUNT_POINT_ROOT}; then
        printf "Can not sync ${EMMC_DEVICE_P1}. Aborting...\n"
        exit -1
    fi
    printf "Syncing rootfs to eMMC done!\n"
}

deploy_bootloader() {
    printf "Updating bootloader... \n"
    [[ -f /mmc_install/MLO ]] || { printf "Can not find /mmc_install/MLO!"; exit -1; }
    [[ -f /mmc_install/u-boot.img ]] || { printf "Can not find /mmc_install/u-boot.img"; exit -1; }

    dd if=/mmc_install/MLO of=${EMMC_DEVICE} bs=512 seek=256 count=256 conv=notrunc || { printf "Failed to deploy MLO!"; exit -1; }
    sync
    dd if=/mmc_install/u-boot.img of=${EMMC_DEVICE} bs=512 seek=768 count=1024 conv=notrunc || { printf "Failed to deploy u-boot.img!"; exit -1; }
    sync
    printf "Updating bootloader done.\n"
}

unmount() {
    printf "Unmounting $1...\n"
    umount $1
    rm -r $1
}

main() {
    if [ -b "${EMMC_DEVICE}" ]; then
        check_if_mounted_and_unmount
    else
        printf "Device \"${EMMC_DEVICE}\" does not seem to be a block device!\n"
        exit -1
    fi

    rewrite_partition
    mount_emmc
    sync_rootfs
    unmount ${EMMC_ROOTFS_DIR}
    deploy_bootloader
}

main
