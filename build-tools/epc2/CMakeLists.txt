SET(SHARED_PACKAGES sudo glibmm libsoup freetype2 libpng png++ boost alsa) 
SET(TARGET_PACKAGES cpupower base linux-rt mc networkmanager nano mkinitcpio mkinitcpio-nfs-utils rsync grub openssh sshfs efibootmgr alsa-tools alsa-utils linux-firmware shared-mime-info)
SET(ALL_PACKAGES ${TARGET_PACKAGES} ${SHARED_PACKAGES})

SET(UPDATE_PACKAGES lm_sensors flac dnsmasq)

string(REPLACE ";" " " SHARED_PACKAGES "${SHARED_PACKAGES}")
string(REPLACE ";" " " TARGET_PACKAGES "${TARGET_PACKAGES}")
string(REPLACE ";" " " ALL_PACKAGES "${ALL_PACKAGES}")
string(REPLACE ";" " " UPDATE_PACKAGES "${UPDATE_PACKAGES}")

SET(PACCACHEDIR pacman-cache-${BUILD_CONTAINER_VERSION})
SET(PACCONFDIR pacman-conf-${BUILD_CONTAINER_VERSION})

add_subdirectory(target-os)
add_subdirectory(install-medium)
add_subdirectory(create-update)
