SET(USER $ENV{USER})
SET(DOCKERNAME nl-cross-build-environment-${USER})
SET(X_GCC nonlinux/output/host/usr/bin/arm-linux-gcc)

EXECUTE_PROCESS(OUTPUT_VARIABLE DOCKER_GROUP_ID COMMAND sh -c "getent group docker | cut -d: -f3")
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
string(STRIP ${DOCKER_GROUP_ID} DOCKER_GROUP_ID)
string(STRIP ${USER_ID} USER_ID)

add_custom_command(
    COMMENT "Create Docker Container for the current User ${USER}"
    OUTPUT .docker-container-created 
    COMMAND docker build --build-arg user=${USER} --build-arg user_id=${USER_ID} --build-arg docker_group_id=${DOCKER_GROUP_ID} -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile 
    COMMAND touch .docker-container-created
)

add_custom_command(
    COMMENT "Clone nonlinux from github"
    OUTPUT .nonlinux-cloned 
    COMMAND git clone -b nonlinear_2020.01 --single-branch https://github.com/nonlinear-labs-dev/nonlinux.git 
    COMMAND touch .nonlinux-cloned
)

add_custom_command(
    COMMENT "Pull nonlinux from github"
    OUTPUT .nonlinux-pulled 
    DEPENDS .nonlinux-cloned 
    WORKING_DIRECTORY nonlinux
    COMMAND git checkout nonlinear_2020.01
    COMMAND git pull origin nonlinear_2020.01
    COMMAND touch ../.nonlinux-pulled
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/build-toolchain.sh 
    "docker run -u ${USER_ID}:${DOCKER_GROUP_ID} \
        --rm \
        -v ${CMAKE_CURRENT_BINARY_DIR}:/workdir \
        -v ${CMAKE_SOURCE_DIR}:/sources \
        ${DOCKERNAME} \
        bash -c '\
            export HOME=/workdir/ccache && \
            cd /workdir/nonlinux && \
            make nonlinear_defconfig && \
            make toolchain'")

add_custom_command(
    COMMENT "Build X-toolchain"
    OUTPUT ${X_GCC} 
    DEPENDS .nonlinux-pulled .docker-container-created 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build-toolchain.sh
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/build-toolchain.sh
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/build-c15.sh 
    "docker run -u ${USER_ID}:${DOCKER_GROUP_ID} \
        --rm \
        -v ${CMAKE_CURRENT_BINARY_DIR}:/workdir \
        -v ${CMAKE_SOURCE_DIR}:/sources \
        ${DOCKERNAME} \
        bash -c '\
            export HOME=/workdir/ccache && \
            cd /workdir/nonlinux && \
            make C15'")

add_custom_command(
    COMMENT "Build BBB C15"
    OUTPUT .bbb-C15-built 
    DEPENDS ${X_GCC}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build-c15.sh
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/build-c15.sh
    COMMAND touch .bbb-C15-built
)

file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/build-rootfs.sh 
    "docker run -u ${USER_ID}:${DOCKER_GROUP_ID} \
        --rm \
        -v ${CMAKE_CURRENT_BINARY_DIR}:/workdir \
        -v ${CMAKE_SOURCE_DIR}:/sources \
        ${DOCKERNAME} \
        bash -c '\
            export HOME=/workdir/ccache && \
            cd /workdir/nonlinux && \
            make C15-dirclean && \
            make'")

add_custom_command(
    COMMENT "Build BBB rootfs"
    OUTPUT .bbb-rootfs-built 
    DEPENDS ${X_GCC}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build-rootfs.sh
    COMMAND /bin/sh ${CMAKE_CURRENT_BINARY_DIR}/build-rootfs.sh
    COMMAND touch .bbb-rootfs-built
)

add_custom_target(bbb-c15 DEPENDS .bbb-C15-built)
add_custom_target(bbb-rootfs DEPENDS .bbb-rootfs-built)
