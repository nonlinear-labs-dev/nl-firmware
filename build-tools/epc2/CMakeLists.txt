SET(NL_UPLOAD_PATH /var/www/html/build-tools2) # used with rsync
SET(NL_DOWNLOAD_PATH /build-tools2) # used with http
SET(ARCH_SERVER "https://archive.archlinux.org/packages/.all")
SET(RT_SERVER "https://pkgbuild.com/~dvzrv/repos/realtime/x86_64/")

# the iso our distro is based on
SET(EPC2_ARCH_LINUX_VERSION "2020.09.01")

SET(EPC2_BASE_ARCH_DOCKERNAME "epc2-base-arch")
SET(EPC2_BASE_ADDITIONAL_PACKAGES "alsa alsa-tools alsa-utils base boost cpupower efibootmgr freetype2 glibmm grub \
    libpng libsoup linux-firmware linux-rt mc mkinitcpio mkinitcpio-nfs-utils nano networkmanager openssh \
    png++ rsync shared-mime-info sudo sshfs")

SET(EPC2_UPDATE_PACKAGES "flac lm_sensors dnsmasq arch-install-scripts")

configure_file(base-arch/Dockerfile base-arch/Dockerfile)
configure_file(update/Dockerfile update/Dockerfile)
configure_file(update/createUpdate.sh update/createUpdate.sh)
configure_file(update/collectUpdatePackages.sh update/collectUpdatePackages.sh)
configure_file(rootfs/hook/nlhook base-arch/hook/nlhook)
configure_file(rootfs/hook/oroot base-arch/hook/oroot)
configure_file(rootfs/install/nlhook base-arch/install/nlhook)
configure_file(rootfs/install/oroot base-arch/install/oroot)

# UR-LINUX
# TODO: simplify collecting packages like it is done for the update container
ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-docker
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/base-arch/Dockerfile
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    COMMAND docker build --build-arg PACKAGES=${EPC2_BASE_ADDITIONAL_PACKAGES} -t ${EPC2_BASE_ARCH_DOCKERNAME} ${CMAKE_CURRENT_BINARY_DIR}/base-arch
    COMMAND touch .epc2-base-os-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting packages installed in our base arch (Ur-Linux)"
    OUTPUT .epc2-base-os-installed-packages
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
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
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
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
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
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
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    COMMAND docker build -t ${EPC2_BUILD_ARCH_DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/base-arch-with-build-tools
    DEPENDS base-arch-with-build-tools/Dockerfile
    COMMAND touch .epc2-build-os-docker
    )

# UPDATE
SET(EPC2_UPDATE_ARCH_DOCKERNAME "epc2-update-arch")

ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding our update arch"
    OUTPUT .epc2-update-os-docker
    DEPENDS .epc2-build-os-docker
    DEPENDS epc2-fetch-nl-resources    
    DEPENDS update/createUpdate.sh
    DEPENDS update/collectUpdatePackages.sh
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/update/Dockerfile
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    # COPY KERNEL UPDATE PACKAGES INTO DOCKER
    COMMAND cp ${DOWNLOAD_DIR}/epc2/packages/linux-rt-5.15.2.19.realtime1-2-x86_64.pkg.tar.zst ${CMAKE_CURRENT_BINARY_DIR}/update
    COMMAND cp ${DOWNLOAD_DIR}/epc2/packages/glibc-2.33-5-x86_64.pkg.tar.zst ${CMAKE_CURRENT_BINARY_DIR}/update
    COMMAND cp ${DOWNLOAD_DIR}/epc2/packages/kmod-29-1-x86_64.pkg.tar.zst ${CMAKE_CURRENT_BINARY_DIR}/update
    COMMAND docker build -t ${EPC2_UPDATE_ARCH_DOCKERNAME} ${CMAKE_CURRENT_BINARY_DIR}/update
    COMMAND touch .epc2-update-os-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Collecting final list of versioned packages installed in our update arch"
    OUTPUT .epc2-update-os-final-packages
    DEPENDS .epc2-update-os-docker
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    VERBATIM
    COMMAND docker run -v ${CMAKE_CURRENT_BINARY_DIR}:/out ${EPC2_UPDATE_ARCH_DOCKERNAME} /collectUpdatePackages.sh ${EPC2_UPDATE_PACKAGES}
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Build epc2 update"
    OUTPUT update.tar
    DEPENDS .epc2-update-os-docker
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DEPENDS ${CMAKE_BINARY_DIR}/projects/web/web.tar.gz
    VERBATIM
    COMMAND docker run 
    --privileged 
    -ti
    -v /var/lib/docker/overlay2:/host-docker
    -v ${CMAKE_SOURCE_DIR}:/srcdir 
    -v ${CMAKE_CURRENT_BINARY_DIR}/update:/bindir 
    -v ${CMAKE_BINARY_DIR}/projects/web:/web
    --env PACKAGES=${EPC2_UPDATE_PACKAGES}
    ${EPC2_UPDATE_ARCH_DOCKERNAME} /createUpdate.sh
    COMMAND cp update/update.tar update.tar
    )

INCLUDE(fetch/fetch.cmake)

ADD_SUBDIRECTORY(rootfs)
ADD_SUBDIRECTORY(install-medium)

ADD_CUSTOM_TARGET(epc2-create-update DEPENDS web update.tar)
