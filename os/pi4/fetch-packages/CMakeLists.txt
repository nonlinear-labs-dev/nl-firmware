cmake_minimum_required(VERSION 3.2)

SET(FOREIGN_URL "https://downloads.raspberrypi.org/raspios_armhf/images/raspios_armhf-2021-11-08/${OS_IMAGE_FILE_NAME}")

configure_file(Dockerfile docker/Dockerfile)
configure_file(fetch.sh docker/fetch.sh)

ADD_CUSTOM_COMMAND(
  COMMENT "Create Docker container for pi4 package fetching"
  DEPENDS Dockerfile
  DEPENDS fetch.sh
  DEPENDS ../CMakeLists.txt
  OUTPUT .build-pi4-fetching-docker-container
  COMMAND docker build -t build-pi4-fetching-docker-container ${CMAKE_CURRENT_BINARY_DIR}/docker
  COMMAND touch .build-pi4-fetching-docker-container)
  
ADD_CUSTOM_COMMAND(
  COMMENT "Fetch Raspberry OS image from foreign servers"
  OUTPUT fetched/${OS_IMAGE_FILE_NAME}
  VERBATIM
  COMMAND mkdir -p fetched
  COMMAND 
    wget ${DOWNLOAD_BASE_URL}/${OS_IMAGE_FILE_NAME} -O ${CMAKE_CURRENT_BINARY_DIR}/fetched/${OS_IMAGE_FILE_NAME} || 
    wget ${FOREIGN_URL} -O ${CMAKE_CURRENT_BINARY_DIR}/fetched/${OS_IMAGE_FILE_NAME}
)

ADD_CUSTOM_COMMAND(
  COMMENT "Fetch Raspberry packages from foreign servers"
  DEPENDS .build-pi4-fetching-docker-container
  OUTPUT .files-fetched
  VERBATIM
  COMMAND mkdir -p fetched    
  COMMAND docker run --privileged -ti --env USER_ID=${USER_ID} --env GROUP_ID=${GROUP_ID}
            -v ${CMAKE_CURRENT_BINARY_DIR}/fetched:/fetched
            -v ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages:/collect-packages
            build-pi4-fetching-docker-container /fetch.sh
  COMMAND touch .files-fetched
)

ADD_CUSTOM_TARGET(pi4-fetch-packages DEPENDS pi4-collect-packages .files-fetched fetched/${OS_IMAGE_FILE_NAME})