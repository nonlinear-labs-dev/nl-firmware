#!/bin/sh
set -x

DEVICE="$1"
DEVICE_P1=""
DEVICE_P2=""
BINARY_DIR=$2
SOURCE_DIR=$3
MOUNT_POINT_ROOT="$2/rootfs-mnt"
MOUNT_POINT_BOOT="$2/boot-mnt"

[ ! -z "$DEVICE" ] || error "You have to specifiy the target device like this: \"BBB_INSTALL_MEDIUM=/dev/sdX make bbb-install-medium\""
#which fatlabel || error "The binary 'fatlabel' has to be installed on the dev pc"

is_mounted() {
    if [ "$(cat /proc/mounts | grep $1)" = "" ]; then
        return 0
    else
        return 1
    fi
}

check_if_mounted_and_unmount() {
    PARTITIONS=$(ls "${DEVICE}"?* 2>/dev/null)

    if [ -n "${PARTITIONS}" ]; then
        printf "Checking mountpoints:\n"
        for partition in ${PARTITIONS}; do
            is_mounted ${partition}
            if [ $? -eq 1 ]; then
                printf "${partition}: is mounted. Unmounting...\n"
                sudo umount "${partition}" 2>/dev/null
            else
            printf "${partition}: is not mounted. Ok...\n"
            fi
        done
    else
        printf "Checking device:\n"
        is_mounted ${DEVICE}
        if [ $? -eq 1 ]; then
            printf "${DEVICE}: is mounted. Unmounting...\n"
            sudo umount "${DEVICE}" 2>/dev/null
        else
        printf "${DEVICE}: is not mounted. Ok...\n"
        fi
    fi
}

create_partitions() {
    DEVICE_P1="${DEVICE}1"
    DEVICE_P2="${DEVICE}2"

    printf "Flushing old partition table...\n"
    sudo dd if=/dev/zero of=${DEVICE} bs=1024 count=1024 2>/dev/null 1>/dev/null && sudo sync

    printf "Creating new partition table...\n"
    printf ',50M,c,*\n,\n' | sudo sfdisk ${DEVICE} && sudo sync
    printf "Creating Partitions... (1/2)\n"
    sudo mkfs.vfat -n BOOT ${DEVICE_P1} 1>/dev/null 2>/dev/null
    printf "Creating Partitions... (1/2)\n"
    sudo mkfs.ext3 ${DEVICE_P2} 1>/dev/null 2>/dev/null
    printf "Rereading partition table...\n"
    sudo partprobe ${DEVICE} && sudo sync
}

mount_boot() {
    printf "Mounting root partition at ${MOUNT_POINT_BOOT}\n"
    sudo mkdir -p ${MOUNT_POINT_BOOT}
    if ! sudo mount ${DEVICE_P1} ${MOUNT_POINT_BOOT}; then
        printf "Can not mount ${DEVICE_P1}. Aborting...\n"
        exit -1
    fi
}

mount_root() {
    printf "Mounting root partition at ${MOUNT_POINT_ROOT}\n"
    sudo mkdir -p ${MOUNT_POINT_ROOT}
    if ! sudo mount ${DEVICE_P2} ${MOUNT_POINT_ROOT}; then
        printf "Can not mount ${DEVICE_P2}. Aborting...\n"
        exit -1
    fi
}

unmount() {
    printf "Unmounting $1...\n"
    sudo umount $1
    sudo rm -r $1
}

sync_rootfs() {
    printf "Syncing rootfs...\n"
    if ! sudo gzip -dc ${BINARY_DIR}/rootfs.tar.gz | sudo tar -C ${MOUNT_POINT_ROOT} -xf -; then
        printf "Can not untar rootfs.tar. Aborting...\n"
        exit -1
    fi
    sudo sync
}

prepare_installation_dir() {
    if ! sudo mkdir ${MOUNT_POINT_ROOT}/install; then
        printf "Cannot create installation directory on root. Aborting...\n"
        exit -1
    fi

    sudo cp ${BINARY_DIR}/MLO ${MOUNT_POINT_ROOT}/install &&
    sudo cp ${BINARY_DIR}/u-boot.img ${MOUNT_POINT_ROOT}/install &&
    sudo cp ${SOURCE_DIR}/runme.sh ${MOUNT_POINT_ROOT}/install
    if [ $? -ne 0 ]; then
        printf "Could not copy insallation files. Aborting...\n"
        exit -1
    fi
    sudo sync
}

write_uenv_file() {
    printf "Rewriting uEnv.txt...\n"
    MMC_CMDS="uenvcmd=mmc rescan"
    MMC_CMDS="${MMC_CMDS}; load mmc 1:1 \${loadaddr} boot/uImage"
    MMC_CMDS="${MMC_CMDS}; load mmc 1:1 \${fdtaddr} boot/nonlinear-labs-2D.dtb"
    MMC_CMDS="${MMC_CMDS}; setenv mmcroot /dev/mmcblk0p1 ro"
    MMC_CMDS="${MMC_CMDS}; setenv mmcrootfstype ext4 rootwait"
    MMC_CMDS="${MMC_CMDS}; setenv bootargs console=\${console} \${optargs} root=\${mmcroot} rootfstype=\${mmcrootfstype}"
    MMC_CMDS="${MMC_CMDS}; bootm \${loadaddr} - \${fdtaddr}\n"
    MMC_CMDS="${MMC_CMDS}uname_r=nonlinear\n"
    MMC_CMDS="${MMC_CMDS}uname_boot=mmc rescan"
    MMC_CMDS="${MMC_CMDS}; load mmc 1:1 \${loadaddr} boot/uImage"
    MMC_CMDS="${MMC_CMDS}; load mmc 1:1 \${fdtaddr} boot/nonlinear-labs-2D.dtb"
    MMC_CMDS="${MMC_CMDS}; setenv mmcroot /dev/mmcblk0p1 ro"
    MMC_CMDS="${MMC_CMDS}; setenv mmcrootfstype ext4 rootwait"
    MMC_CMDS="${MMC_CMDS}; setenv bootargs console=\${console} \${optargs} root=\${mmcroot} rootfstype=\${mmcrootfstype}"
    MMC_CMDS="${MMC_CMDS}; bootm \${loadaddr} - \${fdtaddr}"

    if ! sudo chmod 777 ${MOUNT_POINT_ROOT}/root; then
        printf "Cannot set permission to root. Aborting...\n"
        exit -1
    fi

    if ! sudo rm ${MOUNT_POINT_ROOT}/root/uEnv.txt; then
        printf "Cannot remove old uEnv.txt. Aborting...\n"
        exit -1
    fi

    if ! sudo echo -e "${MMC_CMDS}" > ${MOUNT_POINT_ROOT}/root/uEnv.txt; then
        printf "Cannot create uEnv.txt on root. Aborting...\n"
        exit -1
    fi
    sudo sync
}

sync_bootfs() {
    printf "Syncing bootfs...\n"
    if ! sudo cp ${BINARY_DIR}/u-boot.img ${MOUNT_POINT_BOOT}; then
        printf "Can not copy u-boot.img. Aborting...\n"
        exit -1
    fi

    if ! sudo cp ${BINARY_DIR}/MLO ${MOUNT_POINT_BOOT}; then
        printf "Can not copy MLO. Aborting...\n"
        exit -1
    fi

    printf "Creating uEnv.txt...\n"
    MMC_CMDS="uenvcmd=mmc rescan"
    MMC_CMDS="${MMC_CMDS}; setenv fdtaddr 0x88000000"
    MMC_CMDS="${MMC_CMDS}; load mmc 0:2 \${loadaddr} boot/uImage"
    MMC_CMDS="${MMC_CMDS}; load mmc 0:2 \${fdtaddr} boot/nonlinear-labs-2D.dtb"
    MMC_CMDS="${MMC_CMDS}; setenv mmcroot /dev/mmcblk0p2 ro"
    MMC_CMDS="${MMC_CMDS}; setenv mmcrootfstype ext4 rootwait"
    MMC_CMDS="${MMC_CMDS}; setenv bootargs console=\${console} \${optargs} root=\${mmcroot} rootfstype=\${mmcrootfstype}"
    MMC_CMDS="${MMC_CMDS}; bootm \${loadaddr} - \${fdtaddr}"

    sudo echo "${MMC_CMDS}" > ${BINARY_DIR}/uEnv.txt;
    if ! sudo mv ${BINARY_DIR}/uEnv.txt ${MOUNT_POINT_BOOT}; then
        printf "Can not copy uEnv. Aborting...\n"
        exit -1
    fi
    sudo sync
}

main() {
    if [ -b "${DEVICE}" ]; then
        check_if_mounted_and_unmount
    else
        printf "Device \"$1\" does not seem to be a block device!\n"
        exit -1
    fi

    create_partitions
    mount_root
    sync_rootfs
    prepare_installation_dir
    unmount ${MOUNT_POINT_ROOT}

    mount_boot
    sync_bootfs
    unmount ${MOUNT_POINT_BOOT}

    return 0
}

main
