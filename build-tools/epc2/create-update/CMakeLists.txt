configure_file(Dockerfile Dockerfile)
configure_file(backdoor.sh backdoor.sh)
configure_file(createUpdate.sh createUpdate.sh)
configure_file(installBuildStagePackages.sh installBuildStagePackages.sh)

SET(BASE_KERNEL_FILE linux-rt-${EPC2_BASE_KERNEL}-x86_64.pkg.tar.zst)
SET(UPDATE_KERNEL_FILE linux-rt-${EPC2_UPDATE_KERNEL}-x86_64.pkg.tar.zst)

ADD_CUSTOM_COMMAND(
    COMMENT "Creating Docker container holding the update arch"
    OUTPUT .epc2-create-update-overlay-docker
    DEPENDS createUpdate.sh
    DEPENDS backdoor.sh
    DEPENDS installBuildStagePackages.sh
    DEPENDS Dockerfile
    DEPENDS epc2-download-package-cache
    VERBATIM
    COMMAND rsync -av ${DOWNLOAD_DIR}/epc2/packages ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ../collect-packages/build-packages.txt ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ../collect-packages/base-packages.txt ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND cp ../collect-packages/update-packages.txt ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND docker build -t epc2-create-update-overlay-docker ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND touch .epc2-create-update-overlay-docker
    )

ADD_CUSTOM_COMMAND(
    COMMENT "Build epc2 update"
    OUTPUT update.tar
    DEPENDS .epc2-create-update-overlay-docker
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/CMakeLists.txt
    DEPENDS ${CMAKE_BINARY_DIR}/projects/web/web.tar.gz
    VERBATIM
    COMMAND docker run 
    --privileged -ti
    -v ${CMAKE_BINARY_DIR}:/bindir-root
    -v ${DOWNLOAD_DIR}/epc2/packages:/packages
    -v /var/lib/docker/overlay2:/host-docker
    -v ${CMAKE_SOURCE_DIR}:/srcdir 
    -v ${CMAKE_CURRENT_BINARY_DIR}/update:/bindir 
    -v ${CMAKE_BINARY_DIR}/projects/web:/web
    -v ${CMAKE_CURRENT_BINARY_DIR}/../create-kernel-update:/kernel-update-builddir
    --env PACKAGES=${EPC2_UPDATE_PACKAGES}
    epc2-create-update-overlay-docker /createUpdate.sh
    COMMAND cp update/update.tar update.tar
    )
  
ADD_CUSTOM_TARGET(epc2-create-update 
  DEPENDS web 
  DEPENDS epc2-collect-packages 
  DEPENDS epc2-create-kernel-update
  DEPENDS epc2-download-package-cache 
  DEPENDS update.tar)