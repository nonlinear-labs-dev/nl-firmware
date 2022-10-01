configure_file(download.sh download.sh)

ADD_CUSTOM_COMMAND(
  COMMENT "Download Packages from NL server"
  OUTPUT .files-downloaded
  DEPENDS download.sh
  DEPENDS ../CMakeLists.txt
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/packages.txt
  VERBATIM
  COMMAND ${CMAKE_CURRENT_BINARY_DIR}/download.sh ${CMAKE_CURRENT_BINARY_DIR}/../collect-packages/package-files.txt
  )

ADD_CUSTOM_TARGET(pi4-download-package-cache DEPENDS pi4-collect-packages .files-downloaded)