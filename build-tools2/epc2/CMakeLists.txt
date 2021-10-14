
# the iso our distro is based on
SET(EPC2_ARCH_LINUX_VERSION "2020.09.01")

SET(EPC2_BASE_ARCH_DOCKERNAME "epc2-base-arch")

# WARNING linux-rt  should be added
SET(EPC2_BASE_ADDITIONAL_PACKAGES "sudo cpupower base mkinitcpio mkinitcpio-nfs-utils rsync grub openssh sshfs efibootmgr linux-firmware")

# UR-LINUX
ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-docker
    COMMAND docker build --build-arg PACKAGES=${EPC2_BASE_ADDITIONAL_PACKAGES} -t ${EPC2_BASE_ARCH_DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/base-arch
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/base-arch/Dockerfile
    COMMAND touch .epc2-base-os-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting packages installed in our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-installed-packages
    DEPENDS .epc2-base-os-docker
    VERBATIM
    COMMAND docker run ${EPC2_BASE_ARCH_DOCKERNAME} bash -c "pacman -Q"
    | cut -f 1 -d " "
    > .epc2-base-os-installed-packages
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting dependent packages installed in our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-resolved-packages
    DEPENDS .epc2-base-os-installed-packages
    VERBATIM
    COMMAND cat .epc2-base-os-installed-packages
    | docker run ${EPC2_BASE_ARCH_DOCKERNAME} bash -c "xargs pacman -Qi"
    | grep "Depends On"
    | cut -f2 -d ":"
    | tr " " "\\n"
    | grep -v '^\\\\w*$'
    | sort -u
    | grep -v "\.so="
    > .epc2-base-os-resolved-packages
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting final list of versioned packages installed in our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-final-packages
    DEPENDS .epc2-base-os-resolved-packages
    DEPENDS .epc2-base-os-installed-packages
    VERBATIM
    COMMAND cat .epc2-base-os-installed-packages .epc2-base-os-resolved-packages
    | grep "^[a-zA-Z]"
    | sort -u
    | docker run ${EPC2_BASE_ARCH_DOCKERNAME} xargs pacman -Q
    > .epc2-base-os-final-packages
    )

# UPDATE

# packages to be put into the epc2 update

# maybe also add a variable to set the db pacman should operate on?
SET(EPC2_BASE_OS_UPDATE_PACKAGES "libsoup mc flac")

ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our update arch"
    OUTPUT .epc2-update-os-docker
    DEPENDS .epc2-base-os-docker
    COMMAND docker build --build-arg PACKAGES=${EPC2_BASE_OS_UPDATE_PACKAGES} -t ${EPC2_BASE_ARCH_DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/update-arch
    DEPENDS base-arch/Dockerfile
    DEPENDS update-arch/Dockerfile
    COMMAND touch .epc2-update-os-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting packages installed in our base arch"
    OUTPUT .epc2-update-os-installed-packages
    DEPENDS .epc2-update-os-docker
    VERBATIM
    COMMAND docker run ${EPC2_BASE_ARCH_DOCKERNAME} bash -c "pacman -Q"
    | cut -f 1 -d " "
    > .epc2-update-os-installed-packages
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting dependent packages installed in our update arch"
    OUTPUT .epc2-update-os-resolved-packages
    DEPENDS .epc2-update-os-installed-packages
    VERBATIM
    COMMAND cat .epc2-update-os-installed-packages
    | docker run ${EPC2_BASE_ARCH_DOCKERNAME} bash -c "xargs pacman -Qi"
    | grep "Depends On"
    | cut -f2 -d ":"
    | tr " " "\\n"
    | grep -v '^\\\\w*$'
    | sort -u
    | grep -v "\.so="
    > .epc2-update-os-resolved-packages
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting final list of versioned packages installed in our update arch"
    OUTPUT .epc2-update-os-final-packages
    DEPENDS .epc2-update-os-resolved-packages
    DEPENDS .epc2-update-os-installed-packages
    VERBATIM
    COMMAND cat .epc2-update-os-installed-packages .epc2-update-os-resolved-packages
    | grep "^[a-zA-Z]"
    | sort -u
    | docker run ${EPC2_BASE_ARCH_DOCKERNAME} xargs pacman -Q
    > .epc2-update-os-final-packages
    )

INCLUDE(fetch/fetch.cmake)
ADD_SUBDIRECTORY(rootfs)


# fetch-nl-iso
# fetch-nl-packages
# build-epc2-os
# build-epc2-install-medium
# build-epc2-update

