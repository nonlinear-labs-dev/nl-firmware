configure_file(Dockerfile docker/Dockerfile)
configure_file(DEBIAN/control docker/DEBIAN/control)
configure_file(DEBIAN/postinst docker/DEBIAN/postinst)
configure_file(DEBIAN/postrm docker/DEBIAN/postrm)

EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
STRING(STRIP ${USER_ID} USER_ID)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker Container for cross-building for Raspberry Pi 4" 
    OUTPUT .pi4-docker-container
    DEPENDS Dockerfile
    DEPENDS DEBIAN/control
    DEPENDS DEBIAN/postinst
    DEPENDS DEBIAN/postrm
    COMMAND docker build --build-arg BOB_ID=${USER_ID} -t nl-pi4-crossbuild-environment ${CMAKE_CURRENT_BINARY_DIR}/docker
    COMMAND touch .pi4-docker-container
)

ADD_CUSTOM_COMMAND(
    COMMENT "Run cross-building console for Raspberry Pi 4" 
    OUTPUT .pi4-docker-console
    DEPENDS Dockerfile
    COMMAND docker run -ti -v ${CMAKE_SOURCE_DIR}:/src -v ${CMAKE_CURRENT_BINARY_DIR}:/build nl-pi4-crossbuild-environment bash
)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Userland Packages for Raspberry Pi 4" 
    OUTPUT NL-1.0.deb
    DEPENDS .pi4-docker-container
    COMMAND docker run -ti -v ${CMAKE_SOURCE_DIR}:/src -v ${CMAKE_CURRENT_BINARY_DIR}:/build nl-pi4-crossbuild-environment
)

ADD_CUSTOM_TARGET(pi4-package DEPENDS NL-1.0.deb)
ADD_CUSTOM_TARGET(pi4-crossbuild-console DEPENDS .pi4-docker-console)