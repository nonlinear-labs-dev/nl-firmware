EXECUTE_PROCESS(OUTPUT_VARIABLE GROUP_ID COMMAND sh -c "id -g $USER")
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
string(STRIP ${GROUP_ID} GROUP_ID)
string(STRIP ${USER_ID} USER_ID)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_SOURCE_DIR}/projects/epc/*)

SET(ARCH_VERSION_YEAR 2022)
SET(ARCH_VERSION_MONTH 04)
SET(ARCH_VERSION_DAY 24)
SET(ARCH_VERSION ${ARCH_VERSION_YEAR}${ARCH_VERSION_MONTH}${ARCH_VERSION_DAY}.0.54084)
SET(C15_PACKAGES_VERSION "2022-11-17")
SET(BUILD_SETUP_VERSION "arch-${ARCH_VERSION}-packages-${C15_PACKAGES_VERSION}")
SET(DOCKERNAME pacman-c15-fetch-${BUILD_SETUP_VERSION}-docker)
SET(NL_DOWNLOAD_PATH /build-tools/pacman/c15) # used with http
SET(NL_UPLOAD_PATH /var/www/html/${NL_DOWNLOAD_PATH}) # used with rsync
SET(DOCKER_TAR c15-build-docker-${BUILD_SETUP_VERSION}.tar)

SET(HOST_REQUIREMENTS binutils ccache cmake fakeroot gcc git linux make nano ninja pacman pacman-contrib pkgconfig sudo squashfs-tools xz npm nodejs npm rsync)
STRING(REPLACE ";" " " HOST_REQUIREMENTS "${HOST_REQUIREMENTS}")

SET(C15_REQUIREMENTS alsa alsa-tools alsa-utils arch-install-scripts base boost cpupower dnsmasq flac freetype2 glibmm linux-firmware libpng libsoup lm_sensors mc nano networkmanager png++ shared-mime-info sudo sshfs lsof systemd-sysvcompat vi)
STRING(REPLACE ";" " " C15_REQUIREMENTS "${C15_REQUIREMENTS}")

configure_file(dockerized-create-c15-package.sh dockerized-create-c15-package.sh @ONLY)
configure_file(dockerized-create-c15-rootfs.sh dockerized-create-c15-rootfs.sh @ONLY)
configure_file(Dockerfile.in fetch/Dockerfile @ONLY)

# COMMANDS
ADD_CUSTOM_COMMAND(
  COMMENT "Fetch all resources needed for building a c15 package from arch servers and store it in a docker image"
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/${DOCKER_TAR}
  DEPENDS Dockerfile.in
  VERBATIM
  COMMAND docker build --pull --no-cache ${CMAKE_CURRENT_BINARY_DIR}/fetch -t ${DOCKERNAME}
  COMMAND docker save ${DOCKERNAME} > ${CMAKE_CURRENT_BINARY_DIR}/${DOCKER_TAR}
)

ADD_CUSTOM_COMMAND(
  COMMENT "Upload packed docker image to NL server"
  OUTPUT .upload-docker-image-to-nl-server
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/${DOCKER_TAR}
  VERBATIM
  COMMAND gzip -k -9 ${CMAKE_CURRENT_BINARY_DIR}/${DOCKER_TAR}
  COMMAND rsync --progress ${CMAKE_CURRENT_BINARY_DIR}/${DOCKER_TAR}.gz ${NL_SERVER}:${NL_UPLOAD_PATH}
)

ADD_CUSTOM_COMMAND(
  COMMENT "Build container from NL-served image"
  OUTPUT .arch-docker
  VERBATIM
  COMMAND mkdir -p ${DOWNLOAD_DIR}/${NL_DOWNLOAD_PATH}
  COMMAND test -e ${DOWNLOAD_DIR}/${NL_DOWNLOAD_PATH}/${DOCKER_TAR}.gz || wget http://${NL_SERVER}/${NL_DOWNLOAD_PATH}/${DOCKER_TAR}.gz -O ${DOWNLOAD_DIR}/${NL_DOWNLOAD_PATH}/${DOCKER_TAR}.gz
  COMMAND docker load -i ${DOWNLOAD_DIR}/${NL_DOWNLOAD_PATH}/${DOCKER_TAR}.gz
  COMMAND touch .arch-docker
)

ADD_CUSTOM_COMMAND(
  COMMENT "Build c15 userland package"
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/c15-1.0.0-1-any.pkg.tar.zst
  DEPENDS .arch-docker
  DEPENDS dockerized-create-c15-package.sh
  DEPENDS ${SOURCE_FILES}
  DEPENDS ${CMAKE_BINARY_DIR}/projects/web/web.tar.gz
  VERBATIM
  COMMAND docker run
  -ti --rm
  --network none
  --privileged
  -v ${CMAKE_SOURCE_DIR}:/src
  -v ${CMAKE_CURRENT_BINARY_DIR}:/out
  -v ${CMAKE_CURRENT_BINARY_DIR}:/current-src
  -v ${CMAKE_BINARY_DIR}/projects/web:/web
  ${DOCKERNAME}
  /current-src/dockerized-create-c15-package.sh
)

ADD_CUSTOM_COMMAND(
  COMMENT "Build c15 rootfs package"
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/c15-rootfs.tar.xz
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
  ${DOCKERNAME}
  /current-src/dockerized-create-c15-rootfs.sh
)

ADD_CUSTOM_TARGET(
  c15-pkg
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c15-1.0.0-1-any.pkg.tar.zst
)

ADD_CUSTOM_TARGET(
  c15-rootfs
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c15-rootfs.tar.xz
  DEPENDS web
)

ADD_CUSTOM_TARGET(
  fetch-c15-update-packages
  DEPENDS .upload-docker-image-to-nl-server
)
