# fetch iso and packages from arch servers and uploads the stuff onto NL server

ADD_CUSTOM_COMMAND(
    COMMENT "Fetch Arch ISO image from public (not Nonlinear Labs maintained) servers"
    OUTPUT arch-${EPC2_ARCH_LINUX_VERSION}.iso
    COMMAND wget https://archive.archlinux.org/iso/${EPC2_ARCH_LINUX_VERSION}/archlinux-${EPC2_ARCH_LINUX_VERSION}-x86_64.iso -O arch-${EPC2_ARCH_LINUX_VERSION}.iso
)

SET(ARCH_SERVER "https://archive.archlinux.org/packages/.all")

download script should run in a arch docker and download all the dependencies, too

FOREACH(PACKAGE ${EPC2_PACKAGES})
    SEPARATE_ARGUMENTS(PACKAGE)
    LIST(GET PACKAGE 0 PACKAGE_NAME)
    LIST(GET PACKAGE 1 PACKAGE_VERSION)

    STRING(SHA512 SHA "${PACKAGE_NAME}-${PACKAGE_VERSION}")
    
    ADD_CUSTOM_COMMAND(
      OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/fetched/stamps/${SHA}
      COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/fetched
      COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/fetch/download.sh ${ARCH_SERVER} ${PACKAGE_NAME} ${PACKAGE_VERSION} ${CMAKE_CURRENT_BINARY_DIR}/fetched ${SHA}
    )
    
    LIST(APPEND EPC2_DOWNLOADED_FILES ${CMAKE_CURRENT_BINARY_DIR}/fetched/stamps/${SHA})
ENDFOREACH()

SET(EPC2_UPLOADED_FILES ${EPC2_DOWNLOADED_FILES} arch-${EPC2_ARCH_LINUX_VERSION}.iso)

ADD_CUSTOM_COMMAND(
  OUTPUT .files-uploaded 
  DEPENDS ${EPC2_DOWNLOADED_FILES}
  DEPENDS arch-${EPC2_ARCH_LINUX_VERSION}.iso
  COMMAND echo "done"
  COMMAND rsync -r --progress ${CMAKE_CURRENT_BINARY_DIR}/fetched/ ${NL_SERVER}:${NL_UPLOAD_PATH}
)

ADD_CUSTOM_TARGET(bt2-epc2-fetch-and-upload-resources DEPENDS .files-uploaded)