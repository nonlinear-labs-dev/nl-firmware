
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
    OUTPUT .ap-linux-mounted
    COMMAND mkdir -p AP-Linux-mnt
    COMMAND fuseiso ./AP-Linux-V.4.0.iso ./AP-Linux-mnt
    COMMAND touch .ap-linux-mounted
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
    rm -rf .ap-linux-copy-pimped
    ")

add_custom_command(
    COMMENT "Clean up temps"
    OUTPUT .clean-up
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/clean-temps.sh
)

add_custom_command(
    COMMENT "Copy APLinux"
    DEPENDS .ap-linux-mounted
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
    COMMENT "Pack NonLinuxAP"
    DEPENDS .ap-linux-pimped
    OUTPUT .ap-linux-copy-pimped
    COMMAND rm ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs
    COMMAND mksquashfs ${CMAKE_CURRENT_BINARY_DIR}/squashfs-root ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs
    COMMAND md5sum ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.sfs > ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy/arch/x86_64/airootfs.md5
    COMMAND touch .ap-linux-copy-pimped
)

add_custom_command(
    COMMENT "Create ISO image of NonLinuxAP"
    DEPENDS .ap-linux-copy-pimped
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/AP-Linux-copy
    COMMAND genisoimage -l -r -J -V "ARCH_201704" -b isolinux/isolinux.bin -no-emul-boot -boot-load-size 4 -boot-info-table -c isolinux/boot.cat -o ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso ./
    COMMAND isohybrid ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/start-nonlinux-ap.sh
    "
    qemu-img create -f qcow2 ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.raw 64G
    echo '1' | qemu-system-x86_64 -cdrom ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso -m 2048 -hda ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.raw -serial stdio -nographic
    ")


add_custom_command(
    COMMENT "Create RAW image of NonLinuxAP"
    OUTPUT .nonlinux-raw
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/start-nonlinux-ap.sh
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/NonLinuxAP.iso
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/start-nonlinux-ap.sh
)

add_custom_target(epc-rootfs DEPENDS .nonlinux-raw)
