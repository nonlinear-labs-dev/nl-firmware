ADD_CUSTOM_COMMAND(
    COMMENT "Build Nonlinear Labs binaries in a minmal NonLinux installation"
    DEPENDS epc2-setup-target-os
    DEPENDS createEPC2Update.sh
    DEPENDS createEPC2UpdateDockerCommands.sh
    DEPENDS ${CMAKE_BINARY_DIR}/projects/web/web.tar.gz
    OUTPUT epc2-update.tar
    COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/createEPC2Update.sh ${DOCKERNAME} ${CMAKE_BINARY_DIR} ${CMAKE_SOURCE_DIR} ${UPDATE_PACKAGES}
    )

IF(NOT ${TARGET_PLATFORM} STREQUAL "epc2")
  IF(NOT ${TARGET_PLATFORM} STREQUAL "epc")
    IF(BUILD_WEB)
      ADD_CUSTOM_TARGET(epc2-create-update DEPENDS epc2-update.tar)
      ADD_DEPENDENCIES(epc2-create-update web)
    ENDIF()    
  ENDIF()
ENDIF()