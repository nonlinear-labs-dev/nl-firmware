SET(USER $ENV{USER})
SET(DOCKERNAME nl-cross-build-environment)
SET(NONLINUX nonlinear_2021.47)

file(GLOB_RECURSE ALL_FILES_IN_PROJECT 
    ${CMAKE_SOURCE_DIR}/projects/bbb/*
    ${CMAKE_SOURCE_DIR}/projects/shared/*)

file(GLOB_RECURSE REALTIME_SYSTEM_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/playcontroller/firmware/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${REALTIME_SYSTEM_TARGET_FILES})

file(GLOB_RECURSE SUPERVISOR_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/supervisor/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${SUPERVISOR_TARGET_FILES})

file(GLOB_RECURSE NONMAPS_TARGET_FILES ${CMAKE_SOURCE_DIR}/projects/web/static/nonmaps/target/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${NONMAPS_TARGET_FILES})

add_custom_command(
    COMMENT "Create Docker Container for the current User ${USER}"
    OUTPUT .docker-container-created 
    COMMAND docker build -t ${DOCKERNAME} ${CMAKE_CURRENT_SOURCE_DIR} DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile 
    COMMAND touch .docker-container-created
)

add_custom_command(
    COMMENT "Clone nonlinux from github"
    OUTPUT .nonlinux-cloned 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        " "
        "git config --global user.email bob@the-builder.com"
        "mkdir -p /workdir/fs"
        'git clone https://github.com/nonlinear-labs-dev/nonlinux.git /workdir/fs/nonlinux || true'
        "cd /workdir/fs/nonlinux"
        'git remote add rw git@github.com:nonlinear-labs-dev/nonlinux.git || true'
    COMMAND touch .nonlinux-cloned
)

add_custom_command(
    COMMENT "Pull nonlinux from github"
    OUTPUT .nonlinux-pulled 
    DEPENDS .nonlinux-cloned 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        " "
        "git config --global user.email bob@the-builder.com"
        "cd /workdir/fs/nonlinux"
        "rm -f .gitattributes"
        "git fetch origin"
        "git checkout -f ${NONLINUX}"
        "git pull origin ${NONLINUX}"
)

add_custom_command(
    COMMENT "Setup nonlinux"
    OUTPUT .nonlinux-setup
    DEPENDS .nonlinux-pulled .docker-container-created 
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        "mkdir -p /workdir/fs/ccache"
        "export HOME=/workdir/fs/ccache"
        "cd /workdir/fs/nonlinux"
        "git pull origin ${NONLINUX}"
        "make nonlinear_defconfig"
    COMMAND touch .nonlinux-setup
)

add_custom_command(
    COMMENT "Build BBB rootfs"
    OUTPUT ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz
    DEPENDS .nonlinux-setup
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        "mkdir -p /workdir/fs/ccache"
        "export HOME=/workdir/fs/ccache"
        'rm -rf /workdir/fs/nonlinux/output/target/usr/C15'
        'rm -rf /workdir/fs/nonlinux/output/target/usr/C15-\*'
        "cd /workdir/fs/nonlinux"
        "make C15-clean-for-reconfigure"
        "make C15-rebuild"
        "make playcontroller_driver-clean-for-reconfigure"
        "make playcontroller_driver-rebuild"
        "make espi_driver-clean-for-reconfigure"
        "make espi_driver-rebuild"
        "rm -rf /workdir/fs/nonlinux/output/target/mnt/.not-mounted"
        "make"
        "cp /workdir/fs/nonlinux/output/images/rootfs.tar.gz /workdir/"
        "cp /workdir/fs/nonlinux/output/images/MLO /workdir/"
        "cp /workdir/fs/nonlinux/output/images/u-boot.img /workdir/"
    COMMAND echo "The final rootfs is located at ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz"
)

add_custom_command(
    COMMENT "Starting Buildroot console"
    DEPENDS .nonlinux-setup
    OUTPUT .nonlinux-console
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/runInDocker.sh
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        "mkdir -p /workdir/fs/ccache"
        "export HOME=/workdir/fs/ccache"
        "cd /workdir/fs/nonlinux"
        "bash"
)

add_custom_command(
    COMMENT "Build BBB update"
    OUTPUT ${CMAKE_BINARY_DIR}/nonlinear-bbb-update.tar
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/MLO
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/u-boot.img
    DEPENDS ${SOURCE_FILES}
    COMMAND ${CMAKE_SOURCE_DIR}/build-tools/create-c15-update/create-c15-update.sh
        "unused-epc"
        ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz
        ${CMAKE_BINARY_DIR}/build-tools/bbb/MLO
        ${CMAKE_BINARY_DIR}/build-tools/bbb/u-boot.img
        "unused-playcontroller"
        ${CMAKE_BINARY_DIR}
        ${CMAKE_SOURCE_DIR}
        nonlinear-bbb-update
        bbb
)

add_custom_command(
    COMMENT "Build install medium (to be run with prefix BBB_INSTALL_MEDIUM=/dev/sdX)"
    OUTPUT .bbb-install-medium
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/MLO
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/u-boot.img
    DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/createInstallMedium.sh
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/createInstallMedium.sh
        \${BBB_INSTALL_MEDIUM}
        ${CMAKE_CURRENT_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
)

add_custom_target(bbb-ota-update
    DEPENDS ${CMAKE_BINARY_DIR}/nonlinear-bbb-update.tar
    COMMAND ${CMAKE_SOURCE_DIR}/build-tools/c15-ota-update.sh
        ${CMAKE_BINARY_DIR}/nonlinear-bbb-update.tar
        \${UNIT_NAME}
        \${UNIT_PASSWORD}
    )

add_custom_target(bbb-rootfs DEPENDS ${CMAKE_BINARY_DIR}/build-tools/bbb/rootfs.tar.gz)
add_custom_target(bbb-prepare-build-dir DEPENDS .nonlinux-setup)
add_custom_target(bbb-console DEPENDS .nonlinux-console )
add_custom_target(bbb-install-medium DEPENDS .bbb-install-medium)

