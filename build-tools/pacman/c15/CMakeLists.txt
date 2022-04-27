EXECUTE_PROCESS(OUTPUT_VARIABLE GROUP_ID COMMAND sh -c "id -g $USER")
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
string(STRIP ${GROUP_ID} GROUP_ID)
string(STRIP ${USER_ID} USER_ID)
file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/projects/epc/*)

configure_file(dockerized-create-c15-package.sh dockerized-create-c15-package.sh @ONLY)
configure_file(dockerized-create-c15-rootfs.sh dockerized-create-c15-rootfs.sh @ONLY)

# COMMANDS
ADD_CUSTOM_COMMAND(
    OUTPUT .arch-docker
    DEPENDS Dockerfile
    COMMAND docker build --pull --no-cache ${CMAKE_CURRENT_SOURCE_DIR} -t pacman-c15-docker
    COMMAND touch .arch-docker
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/c15-1.0.0-1-any.pkg.tar.zst
    DEPENDS .arch-docker
    DEPENDS dockerized-create-c15-package.sh
    DEPENDS ${SOURCE_FILES}
    DEPENDS ${CMAKE_BINARY_DIR}/projects/web/web.tar.gz
    VERBATIM
    COMMAND docker run 
        -ti --rm
        --privileged
        -v ${CMAKE_SOURCE_DIR}:/src 
        -v ${CMAKE_CURRENT_BINARY_DIR}:/out
        -v ${CMAKE_CURRENT_BINARY_DIR}:/current-src
        -v ${CMAKE_BINARY_DIR}/projects/web:/web
        pacman-c15-docker 
        /current-src/dockerized-create-c15-package.sh
)

ADD_CUSTOM_COMMAND(
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/c15-rootfs.tar.gz
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c15-1.0.0-1-any.pkg.tar.zst
    DEPENDS dockerized-create-c15-rootfs.sh
    VERBATIM
    COMMAND docker run 
        -ti --rm
        --privileged
        --network none
        -v ${CMAKE_SOURCE_DIR}:/src 
        -v ${CMAKE_CURRENT_BINARY_DIR}:/out
        -v ${CMAKE_CURRENT_BINARY_DIR}:/current-src
        pacman-c15-docker 
        /current-src/dockerized-create-c15-rootfs.sh
)

ADD_CUSTOM_TARGET(
  c15-pkg
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c15-1.0.0-1-any.pkg.tar.zst
  )

ADD_CUSTOM_TARGET(
  c15-rootfs
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c15-rootfs.tar.gz
  DEPENDS web 
  )
