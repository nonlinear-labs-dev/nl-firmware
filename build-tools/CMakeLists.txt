
SET(SHARED_PACKAGES sudo glibmm libsoup freetype2 libpng png++ boost alsa) 
SET(TARGET_PACKAGES cpupower base linux-rt mc networkmanager nano mkinitcpio mkinitcpio-nfs-utils
                    rsync grub openssh sshfs efibootmgr alsa-tools alsa-utils linux-firmware shared-mime-info)
SET(UPDATE_PACKAGES lm_sensors flac dnsmasq)
SET(HOST_PACKAGES nano vi mc arch-install-scripts dosfstools cmake make pkgconf gcc git wget unzip jdk11-openjdk
                   fuse-overlayfs fuse3 pacman-contrib typescript archiso linux mkinitcpio-archiso syslinux npm rsync)
SET(ALL_PACKAGES ${TARGET_PACKAGES} ${SHARED_PACKAGES} ${UPDATE_PACKAGES} ${HOST_PACKAGES})

string(REPLACE ";" " " SHARED_PACKAGES "${SHARED_PACKAGES}")
string(REPLACE ";" " " TARGET_PACKAGES "${TARGET_PACKAGES}")
string(REPLACE ";" " " UPDATE_PACKAGES "${UPDATE_PACKAGES}")
string(REPLACE ";" " " HOST_PACKAGES "${HOST_PACKAGES}")
string(REPLACE ";" " " ALL_PACKAGES "${ALL_PACKAGES}")

add_subdirectory(bbb)
add_subdirectory(build-container)
add_subdirectory(epc)
add_subdirectory(pacman)
add_subdirectory(playcontroller)
add_subdirectory(web)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)

add_custom_command(
    COMMENT "Build C15 update"
    OUTPUT ${CMAKE_BINARY_DIR}/nonlinear-c15-update.tar
    DEPENDS epc-create-update
    DEPENDS bbb-rootfs
    DEPENDS playcontroller-blobs
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/epc/update.tar
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/MLO
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/u-boot.img
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/playcontroller/projects/playcontroller/firmware/src/main/main.bin
    DEPENDS ${SOURCE_FILES}
    COMMAND ${CMAKE_SOURCE_DIR}/build-tools/create-c15-update/create-c15-update.sh
        ${CMAKE_BINARY_DIR}/build-tools/epc/update.tar
        ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz
        ${CMAKE_BINARY_DIR}/build-tools/bbb/MLO
        ${CMAKE_BINARY_DIR}/build-tools/bbb/u-boot.img
        ${CMAKE_BINARY_DIR}/build-tools/playcontroller/projects/playcontroller/firmware/src/main/main.bin
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        nonlinear-c15-update
        "playcontroller epc bbb"
)

add_custom_target(c15-update DEPENDS ${CMAKE_BINARY_DIR}/nonlinear-c15-update.tar)
add_custom_target(c15-ota-update
    DEPENDS ${CMAKE_BINARY_DIR}/nonlinear-c15-update.tar
    COMMAND ${CMAKE_SOURCE_DIR}/build-tools/c15-ota-update.sh
        ${CMAKE_BINARY_DIR}/nonlinear-c15-update.tar
        \${UNIT_NAME}
        \${UNIT_PASSWORD}
    )
