#!/bin/sh

error() {
    echo \"COULD NOT CREATE INSTALL MEDIUM:\"
    echo $1
    exit 1
}

TARGET_DEVICE="$1"
#PARTITION="$TARGET"1
CMAKE_CURRENT_BINARY_DIR=$2
MOUNT_POINT_ROOT="$2/tmp/root"

[ ! -z "$TARGET" ] || error "You have to specifiy the target device like this: \"EPC_INSTALL_MEDIUM=/dev/sdX make bbb-install-medium\""
#which fatlabel || error "The binary 'fatlabel' has to be installed on the dev pc"

function create_partition {
    printf "Umounting ${TARGET_DEVICE}\n"
#    read -p "Press enter to continue..." # for debugging

    printf "Flushing old partition table...\n"
    dd if=/dev/zero of=${TARGET_DEVICE} bs=1024 count=1024 2>/dev/null 1>/dev/null && sync

#	printf "Creating new partition table...\n"
#	echo -e ',50M,c,*\n,\n' | sudo sfdisk ${DEVICE} 2>/dev/null 1>/dev/null && sync

#	printf "Creating Partitions... (1/2)\n"
#	mkfs.vfat -n BOOT ${DEVICE_P1} 1>/dev/null 2>/dev/null

    # or do we need an explicit partition number??
    printf "Creating Partition for rootfs\n"
    mkfs.ext3 ${TARGET_DEVICE} 1>/dev/null 2>/dev/null

    printf "Rereading partition table...\n"
    partprobe ${TARGET_DEVICE} && sync
}


function sync_rootfs {
    printf "Syncing rootfs...\n"
    if ! gzip -dc $CMAKE_CURRENT_BINARY_DIR/rootfs.tar.gz | tar -C ${MOUNT_POINT_ROOT} -xf -; then
        printf "Can not untar rootfs.tar. Aborting...\n"
        exit -1
    fi
    sync
}

# unmount_function
# mkdir -r $MOUNT_POINT_ROOT
# mount_function
# function for deploying runme.sh/MLO/u-boot.img



