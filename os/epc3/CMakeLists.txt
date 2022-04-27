file(GLOB_RECURSE EPC3_SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)

# HOW TO RETRIEVE PACKAGE LIST (maybe automate?):
# docker run -ti archlinux:base-20220424.0.54084 # <-- same as in dockerfile
# echo "[realtime]" >> /etc/pacman.conf
# echo "Server = https://pkgbuild.com/~dvzrv/repos/realtime/x86_64/" >> /etc/pacman.conf
# pacman -Sy
# pacman -S sudo glibmm libsoup freetype2 libpng png++ boost alsa cpupower linux-rt mc networkmanager nano mkinitcpio mkinitcpio-nfs-utils rsync grub openssh sshfs efibootmgr alsa-tools alsa-utils linux-firmware shared-mime-info vi mc arch-install-scripts
# pacman -S <new packages go here>
# pacman -Q # <-- generates list to be copied into packages.txt

# COMMANDS
ADD_CUSTOM_COMMAND(
    OUTPUT .arch-docker
    DEPENDS Dockerfile
    COMMAND docker build --pull --no-cache ${CMAKE_CURRENT_SOURCE_DIR} -t epc3-arch-docker
    COMMAND touch .arch-docker
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/epc3-base-meta-1.0.0-1-any.pkg.tar.zst
    DEPENDS .arch-docker
    DEPENDS install.sh
    DEPENDS hook/nlhook
    DEPENDS hook/oroot
    DEPENDS install/nlhook
    DEPENDS install/oroot
    DEPENDS packages.txt
    DEPENDS dockerized-create-epc3-base-meta-package.sh
    VERBATIM
    COMMAND docker run 
        -ti --rm
        -v ${CMAKE_CURRENT_SOURCE_DIR}:/src 
        -v ${DOWNLOAD_DIR}/os/epc3:/packages
        -v ${CMAKE_CURRENT_BINARY_DIR}:/out
        epc3-arch-docker 
        /src/dockerized-create-epc3-base-meta-package.sh
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/epc3-base.db.tar.zst
    DEPENDS .arch-docker
    DEPENDS packages.txt
    DEPENDS dockerized-create-epc3-base-package-database.sh
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc3-base-meta-1.0.0-1-any.pkg.tar.zst
    VERBATIM
    COMMAND docker run
        -ti --rm
        -v ${CMAKE_CURRENT_SOURCE_DIR}:/src
        -v ${DOWNLOAD_DIR}/os/epc3:/packages
        -v ${CMAKE_CURRENT_BINARY_DIR}:/out
        epc3-arch-docker
        /src/dockerized-create-epc3-base-package-database.sh
    )

ADD_CUSTOM_COMMAND(
    OUTPUT .fetch-base-packages
    DEPENDS packages.txt
    VERBATIM
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/fetched-packages
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch.sh
        "https://archive.archlinux.org/packages/.all https://pkgbuild.com/~dvzrv/repos/realtime/x86_64/"
        ${CMAKE_CURRENT_BINARY_DIR}/fetched-packages
        ${CMAKE_CURRENT_SOURCE_DIR}/packages.txt
)

ADD_CUSTOM_COMMAND(
    OUTPUT epc3-base.iso
    DEPENDS ${EPC3_SOURCE_FILES}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc3-base.db.tar.zst
    VERBATIM
    COMMAND docker run
        -ti --rm
        --privileged
        -v ${CMAKE_CURRENT_SOURCE_DIR}:/src
        -v ${DOWNLOAD_DIR}/os/epc3:/packages
        -v ${CMAKE_CURRENT_BINARY_DIR}:/out
        epc3-arch-docker
        /src/dockerized-create-iso.sh
)

ADD_CUSTOM_COMMAND(
    OUTPUT .download-base-packages
    COMMAND mkdir -p ${DOWNLOAD_DIR}/os/epc3
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch.sh
        http://${NL_SERVER}/os/epc3
        ${DOWNLOAD_DIR}/os/epc3
        ${CMAKE_CURRENT_SOURCE_DIR}/packages.txt
)

# TARGETS
ADD_CUSTOM_TARGET(
    epc3-fetch-base-packages
    DEPENDS .fetch-base-packages
)

ADD_CUSTOM_TARGET(
    epc3-upload-base-packages
    DEPENDS epc3-fetch-base-packages
    COMMAND rsync -rl --progress ${CMAKE_CURRENT_BINARY_DIR}/fetched-packages/ ${NL_SERVER}:/var/www/html/os/epc3
)

ADD_CUSTOM_TARGET(
    epc3-download-base-packages
    DEPENDS .download-base-packages
)

ADD_CUSTOM_TARGET(
    epc3-create-base-meta-package
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc3-base-meta-1.0.0-1-any.pkg.tar.zst
)

ADD_CUSTOM_TARGET(
    epc3-create-base-package-database
    DEPENDS epc3-download-base-packages
    DEPENDS epc3-create-base-meta-package
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc3-base.db.tar.zst
)

ADD_CUSTOM_TARGET(
    epc3-iso
    DEPENDS epc3-create-base-package-database
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/epc3-base.iso
    )
