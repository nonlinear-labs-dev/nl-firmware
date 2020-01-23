
add_custom_command(
    COMMENT "Download APLinux image to ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-V.4.0.iso"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-V.4.0.iso
    COMMAND wget https://netix.dl.sourceforge.net/project/ap-linux/AP-Linux-V.4.0.iso
)

add_custom_command(
    COMMENT "Download update packages"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NonLinux.pkg.tar.gz
    COMMAND wget https://github.com/nonlinear-labs-dev/Audiophile2NonLinux/releases/download/1.0/NonLinux.pkg.tar.gz
)

add_custom_command(
    COMMENT "Mount APLinux"
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-V.4.0.iso
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-mnt/arch/boot/memtest
    COMMAND mkdir -p AP-Linux-mnt
    COMMAND fuseiso ./AP-Linux-V.4.0.iso ./AP-Linux-mnt
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/clean-temps.sh
    "
    chmod -R 777 ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root
    chmod -R 777 ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy
    rm -rf ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy
    rm -rf ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root
    rm -rf .ap-linux-copied
    rm -rf .ap-linux-unsquashed
    rm -rf .ap-linux-pimped
    rm -rf .ap-linux-resquashed
    ")

add_custom_command(
    COMMENT "Clean up temps"
    OUTPUT .clean-up
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/clean-temps.sh
)

add_custom_command(
    COMMENT "Copy APLinux"
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-mnt/arch/boot/memtest
    DEPENDS .clean-up
    OUTPUT .ap-linux-copied
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy
    COMMAND cp -R ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-mnt/* ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/
    COMMAND chmod 777 ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy
)

add_custom_command(
    COMMENT "Unsquash rootfs"
    DEPENDS .ap-linux-copied
    OUTPUT .ap-linux-unsquashed
    COMMAND unsquashfs -no-xattrs ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs
)

add_custom_command(
    COMMENT "Inject transforming scripts into APLinux"
    DEPENDS .ap-linux-unsquashed
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinux.pkg.tar.gz
    OUTPUT .ap-linux-pimped
    COMMAND mkdir ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root/Audiophile2NonLinux
    COMMAND chmod 777 ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root/Audiophile2NonLinux
    COMMAND cp -a
        ${CMAKE_CURRENT_BINARY_DIR}/NonLinux.pkg.tar.gz
        ${CMAKE_CURRENT_SOURCE_DIR}/hook
        ${CMAKE_CURRENT_SOURCE_DIR}/install
        ${CMAKE_CURRENT_SOURCE_DIR}/buildNonlinearLabsBinaries.sh
        ${CMAKE_CURRENT_SOURCE_DIR}/sda.sfdisk
        ${CMAKE_CURRENT_SOURCE_DIR}/createUpdateFromRunningOS.sh
        ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root/Audiophile2NonLinux
    COMMAND cp -a ${CMAKE_CURRENT_SOURCE_DIR}/runme.sh ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root/etc/profile.d/
    COMMAND touch .ap-linux-pimped
)

add_custom_command(
    COMMENT "Re-Squash NonLinuxAP"
    DEPENDS .ap-linux-pimped
    OUTPUT .ap-linux-resquashed
    COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs
    COMMAND mksquashfs ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs
    COMMAND md5sum ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs > ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.md5
    COMMAND touch .ap-linux-resquashed
)

add_custom_command(
    COMMENT "Create ISO image of NonLinuxAP"
    DEPENDS .ap-linux-resquashed
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy
    COMMAND genisoimage -l -r -J -V "ARCH_201704" -b isolinux/isolinux.bin -no-emul-boot -boot-load-size 4 -boot-info-table -c isolinux/boot.cat -o ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso ./
    COMMAND isohybrid ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh
    "
    error() {
        echo \"COULD NOT CREATE INSTALL MEDIUM:\"
        echo $1
        exit 1
    }

    TARGET=\"$1\"
    PARTITION=\"$TARGET\"1

    [ ! -z \"$TARGET\" ] || error \"You have to specifiy the target device like this: \\\"EPC_INSTALL_MEDIUM=/dev/sdX make epc-install-medium\\\"\"
    (lsblk -o PATH,RM | grep \"$TARGET \" | grep \" 1\") || error \"Sepcified device seems NOT to be removeable\"
    which fatlabel || error \"The binary \'fatlabel\' has to be installed on the dev pc\"

    sudo umount ${CMAKE_CURRENT_BINARY_DIR}/NonLinux-mnt
    sudo umount ${CMAKE_CURRENT_BINARY_DIR}/InstallMedium-mnt

    CMD=\"label: dos

label-id: 0x16ed9305
device: $TARGET
unit: sectors

$PARTITION : start= 2048, size= 7687424, type=ef, bootable
\"
    (echo \"$CMD\" | sudo sfdisk $TARGET) || error \"Could not parition target device\"
    sudo mkfs.msdos $PARTITION || error \"Could not mk filesystem on target device\"

    mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/NonLinux-mnt
    mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/InstallMedium-mnt
    sudo mount -o loop ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso ${CMAKE_CURRENT_BINARY_DIR}/NonLinux-mnt || error \"Could not mount source ISO image\"
    sudo mount $PARTITION ${CMAKE_CURRENT_BINARY_DIR}/InstallMedium-mnt || error \"Could not mount target device: $TARGET\"
    sudo cp -a ${CMAKE_CURRENT_BINARY_DIR}/NonLinux-mnt/* ${CMAKE_CURRENT_BINARY_DIR}/InstallMedium-mnt || error \"Could not copy content to target device\"
    sudo umount ${CMAKE_CURRENT_BINARY_DIR}/InstallMedium-mnt || error \"Problems unmounting the target device\"
    sudo fatlabel $PARTITION ARCH_201704 || error \"Could not label the partition correctly, stick will most likely not boot\"
    ")

add_custom_command(
    COMMENT "Build install medium (to be run with prefix EPC_INSTALL_MEDIUM=/dev/sdX)"
    OUTPUT .nonlinux-install-medium
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh \${EPC_INSTALL_MEDIUM}
)

add_custom_target(epc-install-medium DEPENDS .nonlinux-install-medium)
