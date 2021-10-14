# create NL customized rootfs out of arch packages and some tweaks
SET(DOCKERNAME epc2os-create-rootfs-container)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker container for epc2os rootfs creation"
    OUTPUT .${DOCKERNAME}
    DEPENDS Dockerfile
    DEPENDS createRootFS.sh
    COMMAND docker build -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND touch .${DOCKERNAME}
)

ADD_CUSTOM_COMMAND(
    COMMENT "Create epc2os rootfs"
    OUTPUT epc2os.rootfs.tar.gz
    DEPENDS .${DOCKERNAME}
    COMMAND docker run
    --privileged
    -ti
    -v ${DOWNLOAD_DIR}:/downloads
    -v ${CMAKE_CURRENT_SOURCE_DIR}:/current-source-dir
    -v ${CMAKE_CURRENT_BINARY_DIR}:/current-binary-dir
    -v ${CMAKE_CURRENT_BINARY_DIR}/../:/epc2-binary-dir
    ${DOCKERNAME}
)

ADD_CUSTOM_TARGET(bt2-epc2os-rootfs
  DEPENDS epc2os.rootfs.tar.gz
  DEPENDS bt2-epc2-fetch-nl-resources
)
