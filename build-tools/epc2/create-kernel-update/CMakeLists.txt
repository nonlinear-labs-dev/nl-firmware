configure_file(Dockerfile Dockerfile)
configure_file(createUpdate.sh createUpdate.sh)

SET(UPDATE_KERNEL_FILE linux-rt-${EPC2_UPDATE_KERNEL}-x86_64.pkg.tar.zst)

ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container for kernel-update generation"
    OUTPUT .epc2-create-kernel-update-overlay-docker
    DEPENDS createUpdate.sh
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Dockerfile
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../build-rootfs/hook/oroot
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/../build-rootfs/install/oroot
    VERBATIM
    COMMAND rsync -av ${DOWNLOAD_DIR}/epc2/packages/${UPDATE_KERNEL_FILE} ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND rsync -av ${DOWNLOAD_DIR}/epc2/packages/glibc-2.33-5-x86_64.pkg.tar.zst ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND rsync -av ${DOWNLOAD_DIR}/epc2/packages/kmod-29-1-x86_64.pkg.tar.zst ${CMAKE_CURRENT_BINARY_DIR}
    # COMMAND rsync -av ${DOWNLOAD_DIR}/epc2/packages/linux-firmware-20210919.d526e04-1-any.pkg.tar.zst ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/../build-rootfs/hook ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp -R ${CMAKE_CURRENT_SOURCE_DIR}/../build-rootfs/install ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND docker build -t epc2-create-kernel-update-overlay-docker ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND touch .epc2-create-kernel-update-overlay-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Build kernel update"
    OUTPUT update.tar
    DEPENDS .epc2-create-kernel-update-overlay-docker
    VERBATIM
    COMMAND docker run 
    --privileged -ti
    -v ${CMAKE_CURRENT_BINARY_DIR}:/bindir 
    epc2-create-kernel-update-overlay-docker /createUpdate.sh
    )
  
ADD_CUSTOM_TARGET(epc2-create-kernel-update DEPENDS epc2-collect-packages epc2-download-package-cache update.tar)