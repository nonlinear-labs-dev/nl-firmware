EXECUTE_PROCESS(OUTPUT_VARIABLE GROUP_ID COMMAND sh -c "id -g $USER")
STRING(STRIP ${GROUP_ID} GROUP_ID)
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
STRING(STRIP ${USER_ID} USER_ID)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker container for epc2os rootfs creation"
    OUTPUT .build-rootfs-docker-container
    DEPENDS Dockerfile
    DEPENDS createRootFS.sh
    COMMAND docker build -t build-rootfs-docker-container ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND touch .build-rootfs-docker-container
)

ADD_CUSTOM_COMMAND(
    COMMENT "Create epc2os rootfs"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/rootfs.ext4 
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/bootfs.fat
    DEPENDS .build-rootfs-docker-container
    COMMAND docker run
    --privileged -ti --env USER_ID=${USER_ID} --env GROUP_ID=${GROUP_ID}
    -v ${DOWNLOAD_DIR}:/downloads
    -v ${CMAKE_CURRENT_SOURCE_DIR}:/current-source-dir
    -v ${CMAKE_CURRENT_BINARY_DIR}:/current-binary-dir
    -v ${CMAKE_CURRENT_BINARY_DIR}/../:/epc2-binary-dir
    build-rootfs-docker-container
)

ADD_CUSTOM_TARGET(epc2-build-rootfs
  DEPENDS epc2-download-package-cache
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/rootfs.ext4 
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/bootfs.fat
)
