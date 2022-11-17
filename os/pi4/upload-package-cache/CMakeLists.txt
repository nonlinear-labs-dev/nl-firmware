
ADD_CUSTOM_COMMAND(
  COMMENT "Upload fetched Packages to NL server"
  OUTPUT .files-uploaded
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../fetch-packages/.files-fetched
  COMMAND rsync -rl --progress ${CMAKE_CURRENT_BINARY_DIR}/../fetch-packages/fetched/ ${NL_SERVER}:${NL_UPLOAD_PATH}
  )

ADD_CUSTOM_TARGET(pi4-upload-package-cache DEPENDS pi4-fetch-packages .files-uploaded)