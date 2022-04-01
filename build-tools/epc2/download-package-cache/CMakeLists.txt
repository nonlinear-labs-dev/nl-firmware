
ADD_CUSTOM_COMMAND(
  COMMENT "Download Packages from NL server"
  OUTPUT .files-downloaded
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt
  VERBATIM
  COMMAND mkdir -p ${DOWNLOAD_DIR}/epc2/packages
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/download.sh ${DOWNLOAD_BASE_URL} ${DOWNLOAD_DIR}/epc2/packages ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt 
  COMMAND touch .files-downloaded
  )

ADD_CUSTOM_TARGET(epc2-download-package-cache DEPENDS epc2-collect-packages .files-downloaded)