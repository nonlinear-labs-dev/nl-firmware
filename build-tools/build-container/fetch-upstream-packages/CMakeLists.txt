SET(DOCKER_BUILD docker build --no-cache=true --build-arg PACKAGES_TAR=${ARCH_INSTALL_PACKAGES} --build-arg BUILD_CONTAINER_VERSION=${BUILD_CONTAINER_VERSION} --build-arg packages=${HOST_PACKAGES} -t ${DOCKERNAME} ${CMAKE_CURRENT_BINARY_DIR})
SET(ARCH_INSTALL_PACKAGES packages_${BUILD_CONTAINER_VERSION}.tar.gz)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/Dockerfile.in ${CMAKE_CURRENT_BINARY_DIR}/Dockerfile COPYONLY)

ADD_CUSTOM_COMMAND(
    COMMENT "Create docker container running a minimal ARCH based host os"
    OUTPUT .${DOCKERNAME}-created
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/Dockerfile
    COMMAND ${DOCKER_BUILD}
    COMMAND touch .${DOCKERNAME}-created)

# Download the latest packages from official Arch repository - should only be used for creating new NonLinux releases
# for example if new hardware has to be integrated. Normally, the build system has to download prepared, well-defined
# archives of packages to use. This target creates those archives for uploading.
ADD_CUSTOM_COMMAND(
    COMMENT "Create a new NonLinux release (a tar of packages to use) using the latest versions of ARCH packages"
    OUTPUT .fetch-upstream-packages
    DEPENDS .${DOCKERNAME}-created
    DEPENDS createNonLinuxReleasePackages.sh
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/createNonLinuxReleasePackages.sh 
        ${DOCKERNAME} 
        ${CMAKE_CURRENT_BINARY_DIR}/../${ARCH_INSTALL_PACKAGES}
        "${ALL_PACKAGES}"
        ${CMAKE_CURRENT_SOURCE_DIR}
        )
 
ADD_CUSTOM_TARGET(fetch-build-container-upstream-packages DEPENDS .fetch-upstream-packages DEPENDS .${DOCKERNAME}-created)
