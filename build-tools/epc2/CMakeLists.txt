SET(ARCH_VERSION 20200908) # the ARCH version our distribution depens on

SET(SHARED_PACKAGES sudo glibmm libsoup freetype2 libpng png++ boost alsa) 
SET(TARGET_PACKAGES cpupower base linux-rt mc networkmanager nano mkinitcpio mkinitcpio-nfs-utils rsync grub openssh sshfs efibootmgr)
SET(ALL_PACKAGES ${TARGET_PACKAGES} ${SHARED_PACKAGES})

string(REPLACE ";" " " SHARED_PACKAGES "${SHARED_PACKAGES}")
string(REPLACE ";" " " TARGET_PACKAGES "${TARGET_PACKAGES}")
string(REPLACE ";" " " ALL_PACKAGES "${ALL_PACKAGES}")

SET(PACCACHEDIR pacman-cache-${ARCH_VERSION})
SET(PACCONFDIR pacman-conf-${ARCH_VERSION})

add_subdirectory(host-os)
add_subdirectory(target-os)
add_subdirectory(install-medium)
add_subdirectory(create-update)