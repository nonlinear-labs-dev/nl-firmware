# create NL distro out of Arch-ISO, additional packages and some tweaks

SET(DOCKERNAME epc2-create-iso-container)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker container for epc2os.iso creation"
    OUTPUT .${DOCKERNAME}
    DEPENDS Dockerfile
    DEPENDS createIso.sh
    DEPENDS runme.sh
    DEPENDS sda.sfdisk
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../build-rootfs/bootfs.fat
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../build-rootfs/rootfs.ext4
    COMMAND docker build -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND touch .${DOCKERNAME}
)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Nonlinear Labs distro from ArchISO"
    OUTPUT epc2os.iso
    DEPENDS .${DOCKERNAME}
    COMMAND docker run --privileged -ti -v ${CMAKE_CURRENT_SOURCE_DIR}:/src -v ${DOWNLOAD_DIR}:/downloads -v ${CMAKE_CURRENT_BINARY_DIR}/../:/out ${DOCKERNAME}
)

ADD_CUSTOM_TARGET(epc2-create-iso 
  DEPENDS epc2-build-rootfs
  DEPENDS epc2os.iso
)