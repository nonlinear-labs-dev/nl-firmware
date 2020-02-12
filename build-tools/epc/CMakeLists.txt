SET(USER $ENV{USER})
SET(DOCKERNAME nl-epc-build-environment-${USER})

file(GLOB_RECURSE ALL_FILES_IN_PROJECT ${CMAKE_SOURCE_DIR}/projects/*)
file(GLOB_RECURSE NONMAPS_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/NonMaps/target/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${NONMAPS_TARGET_FILES})
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX CMakeLists.txt)

EXECUTE_PROCESS(OUTPUT_VARIABLE DOCKER_GROUP_ID COMMAND sh -c "getent group docker | cut -d: -f3")
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
string(STRIP ${DOCKER_GROUP_ID} DOCKER_GROUP_ID)
string(STRIP ${USER_ID} USER_ID)
file(GLOB_RECURSE EPC_SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/*)

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
    COMMENT "Create NonLinux ISO Image"
    DEPENDS .docker-container-created
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-V.4.0.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinux.pkg.tar.gz
    DEPENDS ${EPC_SCRIPTS}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-script-in-docker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/createNonlinuxISO.sh
)

add_custom_command(
    COMMENT "Install system on virtual machine"
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/disk.raw
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-in-docker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        "qemu-img create -f vmdk /workdir/disk.vmdk 32G"
        "touch serial.log"
        'tail -f serial.log &'
        '(sleep 5 && echo \"1\") | qemu-system-x86_64 -enable-kvm -chardev stdio,id=char0,logfile=serial.log,signal=off -serial chardev:char0 -display none -smp 8 -m 8G -hda /workdir/disk.vmdk -cdrom /workdir/NonLinuxAP.iso'
        "chown ${USER} /workdir/disk.vmdk"
)

add_custom_command(
  COMMENT "Download gwt"
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gwt-2.8.2.zip
  COMMAND wget https://goo.gl/pZZPXS -O ${CMAKE_CURRENT_BINARY_DIR}/gwt-2.8.2.zip)

add_custom_command(
    COMMENT "Create local rootfs"
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-V.4.0.iso
    DEPENDS .docker-container-created
    OUTPUT .nonlinux-local-rootfs
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-script-in-docker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/createLocalRootFS.sh
    COMMAND touch .nonlinux-local-rootfs
)

add_custom_command(
    COMMENT "Create ePC update"
    DEPENDS .nonlinux-local-rootfs
    DEPENDS ${ALL_FILES_IN_PROJECT}
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/epc-update.tar
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/run-script-in-docker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}/createUpdate.sh
)

add_custom_command(
    COMMENT "Build install medium (to be run with prefix EPC_INSTALL_MEDIUM=/dev/sdX)"
    OUTPUT .nonlinux-install-medium
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/create-install-medium.sh \${EPC_INSTALL_MEDIUM}
)

add_custom_target(epc-install-medium DEPENDS .nonlinux-install-medium)
add_custom_target(epc-local-rootfs DEPENDS .nonlinux-local-rootfs)
add_custom_target(epc-create-update DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc-update.tar)
add_custom_target(epc-docker-container DEPENDS .docker-container-created)
add_custom_target(epc-nonlinux-iso DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso)
add_custom_target(epc-nonlinux-vm-installation DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/disk.raw)



