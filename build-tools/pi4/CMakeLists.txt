configure_file(Dockerfile docker/Dockerfile)
configure_file(build.sh docker/build.sh)

file(GLOB_RECURSE ALL_FILES ${CMAKE_SOURCE_DIR}/projects/* ${CMAKE_SOURCE_DIR}/build-tools/* ${CMAKE_SOURCE_DIR}/os/* ${CMAKE_SOURCE_DIR}/cmake/*)

EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
STRING(STRIP ${USER_ID} USER_ID)

ADD_CUSTOM_COMMAND(
    COMMENT "Create Docker Container for cross-building for Raspberry Pi 4" 
    OUTPUT .pi4-docker-container
    DEPENDS Dockerfile
    DEPENDS build.sh
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
    OUTPUT C15-${CMAKE_PROJECT_VERSION}-Linux.deb
    DEPENDS .pi4-docker-container
    DEPENDS ${ALL_FILES}
    DEPENDS ${CMAKE_BINARY_DIR}/projects/web/web.tar.gz
    COMMAND docker run --privileged -ti -v ${CMAKE_SOURCE_DIR}:/src -v ${CMAKE_CURRENT_BINARY_DIR}:/build -v ${CMAKE_BINARY_DIR}/projects/web:/web nl-pi4-crossbuild-environment
)

ADD_CUSTOM_TARGET(pi4-package DEPENDS web C15-${CMAKE_PROJECT_VERSION}-Linux.deb)
ADD_CUSTOM_TARGET(pi4-crossbuild-console DEPENDS .pi4-docker-console)
