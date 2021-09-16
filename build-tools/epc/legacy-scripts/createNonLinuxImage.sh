#!/bin/bash

TIMESTAMP=`date +%m-%d-%Y-%H:%M:%S`
ISO_IN=$1
ISO_OUT=$2
STAGING_DIR=$HOME/NonLinux-repackaging-$TIMESTAMP

error() {
    echo "$1"
    clean_up
    exit
}

check_preconditions() {
    echo $FUNCNAME
    if [ -z $ISO_IN -o -z $ISO_OUT ]; then
        error "usage: $0 path-to-original-AP-Linux-V.4.0.iso path-to-NonLinux-AP-Linux-V.4.0.iso"
    fi

    if [ ! -f $ISO_IN ]; then
        error "Input ISO image does not exist."
    fi

    neededBins=(genisoimage mksquashfs md5sum isohybrid)
    for a in ${neededBins[@]}
    do
	if ! which $a > /dev/null; then 
            error "Please install $a"
	    exit
	fi
    done
    
    echo "Creating ISO $ISO_OUT from $ISO_IN."
}

download_artifacts() {
    if [ ! -f ./NonLinux.pkg.tar.gz ]; then 
        wget http://h2949050.stratoserver.net/epc/NonLinux.pkg.tar.gz
    fi
}

mount_original() {
    echo $FUNCNAME
    mkdir -p $STAGING_DIR/original/p1
    mkdir -p $STAGING_DIR/original/p2
    LOOPDEVICE=$(sudo losetup -f --show -P $ISO_IN)
    sudo mount ${LOOPDEVICE}p1 $STAGING_DIR/original/p1 
    sudo mount ${LOOPDEVICE}p2 $STAGING_DIR/original/p2 
}

create_copy() {
    echo $FUNCNAME
    rm -rf $STAGING_DIR/copy
    cp -a $STAGING_DIR/original $STAGING_DIR/copy
    (cd $STAGING_DIR; sudo unsquashfs $STAGING_DIR/copy/p1/arch/x86_64/airootfs.sfs)
}

modify_copy() {
    echo $FUNCNAME
    sudo mkdir $STAGING_DIR/squashfs-root/Audiophile2NonLinux
    sudo chmod 777 $STAGING_DIR/squashfs-root/Audiophile2NonLinux
    cp -a ./hook ./install ./NonLinux.pkg.tar.gz ./sda.sfdisk $STAGING_DIR/squashfs-root/Audiophile2NonLinux
    sudo cp -a ./runme.sh $STAGING_DIR/squashfs-root/etc/profile.d/
}

create_iso() {
    echo $FUNCNAME
    rm $STAGING_DIR/copy/p1/arch/x86_64/airootfs.sfs
    sudo mksquashfs $STAGING_DIR/squashfs-root $STAGING_DIR/copy/p1/arch/x86_64/airootfs.sfs
    sudo rm -rf $STAGING_DIR/squashfs-root
    md5sum $STAGING_DIR/copy/p1/arch/x86_64/airootfs.sfs > $STAGING_DIR/copy/p1/arch/x86_64/airootfs.md5
    (cd $STAGING_DIR/copy/p1; sudo genisoimage -l -r -J -V "ARCH_201704" -b isolinux/isolinux.bin -no-emul-boot -boot-load-size 4 -boot-info-table -c isolinux/boot.cat -o $ISO_OUT ./)
    sudo isohybrid $ISO_OUT
    sudo chown $USER $ISO_OUT
}

unmount_original() {
    echo $FUNCNAME
    if [ -d $STAGING_DIR/original ]; then
        sudo umount $STAGING_DIR/original/p1
	sudo umount $STAGING_DIR/original/p2
    fi
}

clean_up() {
    unmount_original
    sudo rm -rf $STAGING_DIR
}

main() {
    check_preconditions
    download_artifacts
    mount_original
    create_copy
    modify_copy
    create_iso
    clean_up
}

main
