
ADD_CUSTOM_COMMAND(
    COMMENT "Create iso for installing a minimal nonlinux ${BUILD_CONTAINER_VERSION}"
    OUTPUT nonlinux-${BUILD_CONTAINER_VERSION}.iso
    DEPENDS epc2-setup-target-os
    DEPENDS createInstallMedium.sh
    DEPENDS createInstallMediumDockerCommands.sh
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/createInstallMedium.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_CURRENT_BINARY_DIR}/../target-os ${BUILD_CONTAINER_VERSION}
)

ADD_CUSTOM_TARGET(epc2-create-install-medium DEPENDS nonlinux-${BUILD_CONTAINER_VERSION}.iso)