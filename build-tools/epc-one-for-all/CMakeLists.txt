ADD_CUSTOM_COMMAND(
    COMMENT "Build epc-one-for-all update"
    OUTPUT update.tar
    DEPENDS createUpdate.sh
    DEPENDS ${CMAKE_BINARY_DIR}/build-tools/pacman/c15/c15-rootfs.tar.xz
    DEPENDS post-install.sh
    DEPENDS C15.nmconnection
    DEPENDS wpa_supplicant.conf
    VERBATIM
    COMMAND test -e ${DOWNLOAD_DIR}/xz-5.2.5-linux-x86_64.tar.gz || 
      wget ${DOWNLOAD_LOCATION}/build-tools2/xz-5.2.5-linux-x86_64.tar.gz -O ${DOWNLOAD_DIR}/xz-5.2.5-linux-x86_64.tar.gz
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/createUpdate.sh 
      ${CMAKE_CURRENT_BINARY_DIR}
      ${CMAKE_BINARY_DIR}/build-tools/pacman/c15/c15-rootfs.tar.xz
      ${DOWNLOAD_DIR}/xz-5.2.5-linux-x86_64.tar.gz
      ${CMAKE_CURRENT_SOURCE_DIR}/post-install.sh
      ${CMAKE_CURRENT_SOURCE_DIR}/C15.nmconnection
      ${CMAKE_CURRENT_SOURCE_DIR}/wpa_supplicant.conf
    )
  
ADD_CUSTOM_TARGET(epc-one-for-all-create-update 
  DEPENDS c15-rootfs
  DEPENDS update.tar)