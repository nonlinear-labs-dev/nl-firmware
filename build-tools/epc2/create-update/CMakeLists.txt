ADD_CUSTOM_COMMAND(
    COMMENT "Build Nonlinear Labs binaries in a minmal NonLinux installation"
    DEPENDS epc2-setup-target-os
    OUTPUT overlay.ext4
    COMMAND ${DOCKER_RUN} 
        ${CMAKE_CURRENT_BINARY_DIR}
        "pacman --noconfirm --overwrite '\\\*' -S ${INSTALL_PACKAGES}"
        'yes | truncate -s 8G /work/rootfs.ext4'
        'yes | truncate -s 512M /work/bootfs.fat'
        'yes | mkfs.ext4 /work/rootfs.ext4'
        'yes | mkfs.fat /work/bootfs.fat'
        "pacman --noconfirm -Sw ${TARGET_PACKAGES}"
        "mount -o loop /work/rootfs.ext4 /mnt"
        "mkdir /mnt/boot"
        "mount -o loop /work/bootfs.fat /mnt/boot/"
        'echo Server\ =\ file:///var/lib/pacman/sync > /etc/pacman.d/mirrorlist'
        'echo [dvzrv] >> /etc/pacman.d/mirrorlist'
        'echo Server\ =\ file:///var/lib/pacman/sync >> /etc/pacman.d/mirrorlist'
        "pacstrap -c /mnt base ${TARGET_PACKAGES}"
        # TODO: hooks installieren
        # User anlegen
        # ssh Zugriff
)

ADD_CUSTOM_TARGET(epc2-create-update DEPENDS rootfs.ext4)