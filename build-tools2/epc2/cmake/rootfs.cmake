# create NL customized rootfs out of arch packages and some tweaks
SET(DOCKERNAME epc2os-create-rootfs-container)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker container for epc2os rootfs creation"
    OUTPUT .${DOCKERNAME}
    DEPENDS rootfs/Dockerfile
    DEPENDS rootfs/createRootFS.sh
    COMMAND docker build -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/rootfs
    COMMAND touch .${DOCKERNAME}
)

FOREACH(PACKAGE ${EPC2_PACKAGES})
    SEPARATE_ARGUMENTS(PACKAGE)
    LIST(GET PACKAGE 0 PACKAGE_NAME)
    LIST(GET PACKAGE 1 PACKAGE_VERSION)

    LIST(APPEND EPC2_PACKAGES_ENV ${PACKAGE_NAME}-${PACKAGE_VERSION})
ENDFOREACH()

ADD_CUSTOM_COMMAND(
    COMMENT "Create epc2os rootfs"
    OUTPUT epc2os.rootfs.tar.gz
    DEPENDS .${DOCKERNAME}
    COMMAND docker run --privileged -ti --env PACKAGES="${EPC2_PACKAGES_ENV}" -v ${DOWNLOAD_DIR}:/downloads -v ${CMAKE_CURRENT_BINARY_DIR}:/out ${DOCKERNAME}
)

ADD_CUSTOM_TARGET(bt2-epc2os-rootfs
  DEPENDS epc2os.rootfs.tar.gz
  DEPENDS bt2-epc2-fetch-nl-resources
)