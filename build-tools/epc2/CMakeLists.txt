SET(ARCH_VERSION 20200908) # the ARCH version our distribution depens on

SET(SHARED_PACKAGES glibmm libsoup freetype2 libpng png++) 
SET(TARGET_PACKAGES cpupower base linux-rt mc networkmanager nano)
SET(HOST_PACKAGES git cmake make gcc pkgconf jdk11-openjdk boost arch-install-scripts dosfstools)
SET(ALL_PACKAGES ${HOST_PACKAGES} ${TARGET_PACKAGES} ${SHARED_PACKAGES})

string(REPLACE ";" " " SHARED_PACKAGES "${SHARED_PACKAGES}")
string(REPLACE ";" " " TARGET_PACKAGES "${TARGET_PACKAGES}")
string(REPLACE ";" " " HOST_PACKAGES "${HOST_PACKAGES}")
string(REPLACE ";" " " ALL_PACKAGES "${ALL_PACKAGES}")

SET(PACCACHEDIR pacman-cache-${ARCH_VERSION})
SET(PACCONFDIR pacman-conf-${ARCH_VERSION})

add_subdirectory(host-os)
add_subdirectory(target-os)
add_subdirectory(install-medium)
add_subdirectory(create-update)