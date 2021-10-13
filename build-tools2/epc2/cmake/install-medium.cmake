# create NL distro out of Arch-ISO, additional packages and some tweaks

SET(DOCKERNAME epc2-create-iso-container)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker container for epc2os.iso creation"
    OUTPUT .${DOCKERNAME}
    DEPENDS install-medium/Dockerfile
    DEPENDS install-medium/createIso.sh
    COMMAND docker build -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR}/install-medium
    COMMAND touch .${DOCKERNAME}
)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Nonlinear Labs distro from ArchISO"
    OUTPUT epc2os.iso
    DEPENDS .${DOCKERNAME}
    COMMAND docker run -v ${DOWNLOAD_DIR}:/downloads -v ${CMAKE_CURRENT_BINARY_DIR}:/out ${DOCKERNAME}
)

ADD_CUSTOM_TARGET(bt2-epc2-create-iso 
  DEPENDS epc2os.iso
  DEPENDS bt2-epc2-fetch-nl-resources
)