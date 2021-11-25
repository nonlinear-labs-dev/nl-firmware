# fetch packages from arch servers and uploads the stuff onto NL server

SET(DOWNLOAD_BASE_URL http://${NL_SERVER}${NL_DOWNLOAD_PATH})

ADD_CUSTOM_COMMAND(
  COMMENT "Fetch base packages from NL/Arch/RT servers"
  OUTPUT .base-files-fetched
  DEPENDS .epc2-base-os-final-packages
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/fetched
  # download the resources from our server first
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${DOWNLOAD_BASE_URL} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/.epc2-base-os-final-packages no-fail
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${ARCH_SERVER} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/.epc2-base-os-final-packages
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${RT_SERVER} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/.epc2-base-os-final-packages
)

ADD_CUSTOM_COMMAND(
  COMMENT "Fetch update packages from NL/Arch/RT servers"
  OUTPUT .update-files-fetched
  DEPENDS .epc2-update-os-final-packages
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/fetched
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${DOWNLOAD_BASE_URL} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/.epc2-update-os-final-packages no-fail
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${ARCH_SERVER} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/.epc2-update-os-final-packages
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${RT_SERVER} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${CMAKE_CURRENT_BINARY_DIR}/.epc2-update-os-final-packages
)

ADD_CUSTOM_COMMAND(
  COMMENT "Upload packages to NL servers"
  OUTPUT .files-uploaded
  DEPENDS .base-files-fetched
  DEPENDS .update-files-fetched
  VERBATIM
  COMMAND docker run 
  -v ${CMAKE_CURRENT_BINARY_DIR}/fetched:/nonlinux 
  archlinux:20200908 
  bash -c "cd / && repo-add -q /nonlinux/nonlinux.db.tar.gz /nonlinux/*.xz /nonlinux/*.zst"
  COMMAND rsync -r --progress ${CMAKE_CURRENT_BINARY_DIR}/fetched/ ${NL_SERVER}:${NL_UPLOAD_PATH}
)

# fetch packages from NL server
ADD_CUSTOM_COMMAND(
  COMMENT "Fetch packages from nl servers"
  OUTPUT .nl-files-fetched
  OUTPUT ${DOWNLOAD_DIR}/epc2/packages/nonlinux.files.tar.gz
  OUTPUT ${DOWNLOAD_DIR}/epc2/packages/nonlinux.db.tar.gz
  DEPENDS .epc2-base-os-final-packages
  DEPENDS .epc2-update-os-final-packages
  COMMAND mkdir -p ${DOWNLOAD_DIR}/epc2/packages
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${DOWNLOAD_BASE_URL} ${DOWNLOAD_DIR}/epc2/packages ${CMAKE_CURRENT_BINARY_DIR}/.epc2-base-os-final-packages
  COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${DOWNLOAD_BASE_URL} ${DOWNLOAD_DIR}/epc2/packages ${CMAKE_CURRENT_BINARY_DIR}/.epc2-update-os-final-packages
  COMMAND wget -q  ${DOWNLOAD_BASE_URL}/nonlinux.db.tar.gz -O ${DOWNLOAD_DIR}/epc2/packages/nonlinux.db.tar.gz
  COMMAND wget -q  ${DOWNLOAD_BASE_URL}/nonlinux.files.tar.gz -O ${DOWNLOAD_DIR}/epc2/packages/nonlinux.files.tar.gz
  COMMAND ln -s ./nonlinux.files.tar.gz ${DOWNLOAD_DIR}/epc2/packages/nonlinux.files || true
  COMMAND ln -s ./nonlinux.db.tar.gz ${DOWNLOAD_DIR}/epc2/packages/nonlinux.db || true
  COMMAND touch .nl-files-fetched
)

ADD_CUSTOM_TARGET(epc2-fetch-and-upload-resources DEPENDS .files-uploaded)
ADD_CUSTOM_TARGET(epc2-fetch-nl-resources DEPENDS .nl-files-fetched)
