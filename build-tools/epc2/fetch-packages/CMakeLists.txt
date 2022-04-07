CONFIGURE_FILE(download.sh download.sh @ONLY)

SET(DOWNLOAD_BASE_URL http://${NL_SERVER}${NL_DOWNLOAD_PATH})

ADD_CUSTOM_COMMAND(
  COMMENT "Fetch packages from NL/Arch/RT servers"
  OUTPUT .files-fetched
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt
  COMMAND mkdir -p fetched
  VERBATIM
  COMMAND 
  ./download.sh ${DOWNLOAD_BASE_URL} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt ||
  ./download.sh ${ARCH_SERVER} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt ||
  ./download.sh ${RT_SERVER} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt
  COMMAND touch .files-fetched
)

ADD_CUSTOM_TARGET(epc2-fetch-packages DEPENDS epc2-collect-packages .files-fetched)