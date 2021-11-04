SET(PACCACHEDIR pacman-cache-${BUILD_CONTAINER_VERSION})
SET(PACCONFDIR pacman-conf-${BUILD_CONTAINER_VERSION})

add_subdirectory(target-os)
add_subdirectory(install-medium)
add_subdirectory(create-update)
