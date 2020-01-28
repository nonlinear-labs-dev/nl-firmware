SET(USER $ENV{USER})
SET(DOCKERNAME nl-epc-build-environment-${USER})

file(GLOB_RECURSE ALL_FILES_IN_PROJECT ${CMAKE_SOURCE_DIR}/projects/*)

EXECUTE_PROCESS(OUTPUT_VARIABLE DOCKER_GROUP_ID COMMAND sh -c "getent group docker | cut -d: -f3")
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
string(STRIP ${DOCKER_GROUP_ID} DOCKER_GROUP_ID)
string(STRIP ${USER_ID} USER_ID)

add_custom_command(
    COMMENT "Create Docker Container for the current User ${USER}"
    OUTPUT .docker-container-created
    COMMAND docker build --build-arg user=${USER} --build-arg user_id=${USER_ID} --build-arg docker_group_id=${DOCKER_GROUP_ID} -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile
    COMMAND touch .docker-container-created
)

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
    COMMENT "Create ISO image of NonLinuxAP"
    DEPENDS .docker-container-created
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-V.4.0.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinux.pkg.tar.gz
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-in-docker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        "mkdir -p /internal/AP-Linux-mnt"
        "mount -o loop /workdir/AP-Linux-V.4.0.iso /internal/AP-Linux-mnt"
        "mkdir -p /internal/AP-Linux-copy"
        "cp -R /internal/AP-Linux-mnt/* /internal/AP-Linux-copy/"
        "chmod 777 /internal/AP-Linux-copy"
        "unsquashfs -no-xattrs /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs"
        "mkdir ./squashfs-root/Audiophile2NonLinux"
        "chmod 777 ./squashfs-root/Audiophile2NonLinux"
        "cp -a /workdir/NonLinux.pkg.tar.gz /sources/hook /sources/install /sources/buildNonlinearLabsBinaries.sh /sources/sda.sfdisk /sources/createUpdateFromRunningOS.sh ./squashfs-root/Audiophile2NonLinux"
        "cp -a /sources/runme.sh ./squashfs-root/etc/profile.d/"
        "rm /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs"
        "mksquashfs ./squashfs-root /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs"
        'md5sum /internal/AP-Linux-copy/arch/x86_64/airootfs.sfs > /internal/AP-Linux-copy/arch/x86_64/airootfs.md5'
        "genisoimage -l -r -J -V \"ARCH_201704\" -b isolinux/isolinux.bin -no-emul-boot -boot-load-size 4 -boot-info-table -c isolinux/boot.cat -o /internal/NonLinuxAP.iso /internal/AP-Linux-copy/"
        "isohybrid /internal/NonLinuxAP.iso"
        "mv /internal/NonLinuxAP.iso /workdir/NonLinuxAP.iso"
        "chown ${USER} /workdir/NonLinuxAP.iso"
)

add_custom_command(
    COMMENT "Create ePC update"
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/epc-update.tar
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-in-docker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        "mkdir -p /internal"
        "qemu-img create -f vmdk /internal/disk.vmdk 64G"
        '(sleep 5 && echo \"1\") | qemu-system-x86_64 -nographic -cdrom /workdir/NonLinuxAP.iso -m 2048 -hda /internal/disk.vmdk'
        "bash"
)



add_custom_command(
    COMMENT "Build install medium (to be run with prefix EPC_INSTALL_MEDIUM=/dev/sdX)"
    OUTPUT .nonlinux-install-medium
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh \${EPC_INSTALL_MEDIUM}
)

add_custom_target(epc-install-medium DEPENDS .nonlinux-install-medium)
add_custom_target(epc-create-update DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc-update.tar)
