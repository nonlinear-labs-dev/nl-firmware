
# the iso our distro is based on
SET(EPC2_ARCH_LINUX_VERSION "2020.09.01")

SET(EPC2_BASE_ARCH_DOCKERNAME "epc2-base-arch")
SET(EPC2_BASE_ADDITIONAL_PACKAGES "sudo cpupower base mc mkinitcpio mkinitcpio-nfs-utils rsync grub openssh sshfs efibootmgr linux-firmware linux-rt alsa networkmanager nano alsa-tools alsa-utils shared-mime-info glibmm libsoup freetype2 libpng png++ boost alsa")

# UR-LINUX
ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-docker
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/base-arch/Dockerfile
    COMMAND docker build --build-arg PACKAGES=${EPC2_BASE_ADDITIONAL_PACKAGES} -t ${EPC2_BASE_ARCH_DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/base-arch
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

# WITH BUILD TOOLS
SET(EPC2_BUILD_ARCH_DOCKERNAME "epc2-build-arch")

ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our build arch"
    OUTPUT .epc2-build-os-docker
    DEPENDS .epc2-base-os-docker
    COMMAND docker build -t ${EPC2_BUILD_ARCH_DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/base-arch-with-build-tools
    DEPENDS base-arch-with-build-tools/Dockerfile
    COMMAND touch .epc2-build-os-docker
    )

# UPDATE

SET(EPC2_UPDATE_ARCH_DOCKERNAME "epc2-update-arch")
SET(EPC2_UPDATE_PACKAGES "libsoup flac lm_sensors dnsmasq")

ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our update arch"
    OUTPUT .epc2-update-os-docker
    DEPENDS .epc2-base-os-docker
    DEPENDS update/createUpdate.sh
    DEPENDS update/Dockerfile
    COMMAND docker build --build-arg PACKAGES=${EPC2_UPDATE_PACKAGES} -t ${EPC2_UPDATE_ARCH_DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/update
    COMMAND touch .epc2-update-os-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting packages installed in our update arch"
    OUTPUT .epc2-update-os-installed-packages
    DEPENDS .epc2-update-os-docker
    VERBATIM
    COMMAND docker run ${EPC2_UPDATE_ARCH_DOCKERNAME} bash -c "pacman -Q"
    | cut -f 1 -d " "
    > .epc2-update-os-installed-packages
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting dependent packages installed in our update arch"
    OUTPUT .epc2-update-os-resolved-packages
    DEPENDS .epc2-update-os-installed-packages
    VERBATIM
    COMMAND cat .epc2-update-os-installed-packages
    | docker run ${EPC2_UPDATE_ARCH_DOCKERNAME} bash -c "xargs pacman -Qi"
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
    | docker run ${EPC2_UPDATE_ARCH_DOCKERNAME} xargs pacman -Q
    > .epc2-update-os-final-packages
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Build epc2 update"
    OUTPUT epc2-update.tar
    DEPENDS .epc2-update-os-docker
    VERBATIM
    COMMAND docker run -ti -v ${CMAKE_SOURCE_DIR}:/srcdir -v ${CMAKE_CURRENT_BINARY_DIR}/update:/bindir ${EPC2_UPDATE_ARCH_DOCKERNAME} /createUpdate.sh
    )

INCLUDE(fetch/fetch.cmake)

ADD_SUBDIRECTORY(rootfs)
ADD_SUBDIRECTORY(install-medium)


ADD_CUSTOM_TARGET(hook DEPENDS epc2-update.tar)