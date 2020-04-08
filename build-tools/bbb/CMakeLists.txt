SET(USER $ENV{USER})
SET(DOCKERNAME nl-cross-build-environment-${USER})

file(GLOB_RECURSE ALL_FILES_IN_PROJECT ${CMAKE_SOURCE_DIR}/projects/*)

file(GLOB_RECURSE REALTIME_SYSTEM_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/realtime-system/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${REALTIME_SYSTEM_TARGET_FILES})

file(GLOB_RECURSE SUPERVISOR_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/supervisor/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${SUPERVISOR_TARGET_FILES})

file(GLOB_RECURSE NONMAPS_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/NonMaps/target/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${NONMAPS_TARGET_FILES})

EXECUTE_PROCESS(OUTPUT_VARIABLE GROUP_ID COMMAND sh -c "id -g $USER")
EXECUTE_PROCESS(OUTPUT_VARIABLE ROOT_USER_ID COMMAND sh -c "id -u root")
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")

string(STRIP ${GROUP_ID} GROUP_ID)
string(STRIP ${ROOT_USER_ID} ROOT_USER_ID)
string(STRIP ${USER_ID} USER_ID)

add_custom_command(
    COMMENT "Create Docker Container for the current User ${USER}"
    OUTPUT .docker-container-created 
    COMMAND docker build --build-arg user=${USER} --build-arg user_id=${USER_ID} --build-arg group_id=${GROUP_ID} -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile 
    COMMAND touch .docker-container-created
)

add_custom_command(
    COMMENT "Create filesystem"
    OUTPUT  ${CMAKE_CURRENT_BINARY_DIR}/fs.ext4
    COMMAND truncate -s 10G ${CMAKE_CURRENT_BINARY_DIR}/fs.ext4
    COMMAND mkfs.ext4 ${CMAKE_CURRENT_BINARY_DIR}/fs.ext4
)

add_custom_command(
    COMMENT "Clone nonlinux from github"
    OUTPUT .nonlinux-cloned 
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/fs.ext4
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${ROOT_USER_ID}
        ${GROUP_ID}
        "mkdir -p /fs"
        "mount -o loop /workdir/fs.ext4 /fs"
        "git clone -b nonlinear_2020.01 --single-branch https://github.com/nonlinear-labs-dev/nonlinux.git /fs/nonlinux"
        "cd /fs/nonlinux"
        "git pull origin nonlinear_2020.01"
)

add_custom_command(
    COMMENT "Pull nonlinux from github"
    OUTPUT .nonlinux-pulled 
    DEPENDS .nonlinux-cloned 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${ROOT_USER_ID}
        ${GROUP_ID}
        "set -x"
        "mkdir -p /fs"
        "mount -o loop /workdir/fs.ext4 /fs"
        "cd /fs/nonlinux"
        "git checkout nonlinear_2020.01"
        "git pull origin nonlinear_2020.01"
)

add_custom_command(
    COMMENT "Setup nonlinux"
    OUTPUT .nonlinux-setup
    DEPENDS .nonlinux-pulled .docker-container-created 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${ROOT_USER_ID}
        ${GROUP_ID}
        "export HOME=/workdir/ccache"
        "mkdir -p /fs"
        "mount -o loop /workdir/fs.ext4 /fs"
        "cd /fs/nonlinux"
        "git pull origin nonlinear_2020.01"
        "make nonlinear_defconfig"
    COMMAND touch .nonlinux-setup
)

add_custom_command(
    COMMENT "Update C15 dir within nonlinux build directory"
    OUTPUT .nonlinux-update
    DEPENDS .nonlinux-setup
    DEPENDS ${ALL_FILES_IN_PROJECT}
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${ROOT_USER_ID}
        ${GROUP_ID}
        "export HOME=/workdir/ccache"
        "mkdir -p /fs"
        "mount -o loop /workdir/fs.ext4 /fs"
        "rsync -av --delete --exclude=projects/NonMaps/target/* /sources/projects /fs/nonlinux/output/build/C15-master"
    COMMAND touch .nonlinux-update
)

add_custom_command(
    COMMENT "Build BBB rootfs"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nonlinux/output/images/rootfs.tar.gz
    DEPENDS .nonlinux-update
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        ${ROOT_USER_ID}
        ${GROUP_ID}
        "export HOME=/workdir/ccache"
        "mkdir -p /fs"
        "mount -o loop /workdir/fs.ext4 /fs"
        'rm -rf /fs/nonlinux/output/target/usr/C15'
        'rm -rf /fs/nonlinux/output/target/usr/C15-\*'
        "cd /fs/nonlinux"
        "make C15-clean-for-reconfigure"
        "make C15-rebuild"
        "make"
        "cp /fs/nonlinux/output/images/rootfs.tar.gz /workdir/"
    COMMAND echo "The final rootfs is located at ${CMAKE_CURRENT_BINARY_DIR}/rootfs.tar.gz"
)

add_custom_target(bbb-rootfs DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nonlinux/output/images/rootfs.tar.gz)
add_custom_target(bbb-prepare-build-dir DEPENDS .nonlinux-setup)
