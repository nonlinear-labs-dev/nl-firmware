OPTION(BUILD_EPC "Build an ePC image" Off)
OPTION(BUILD_BBB "Build an BeagleBoneBlack image" Off)

IF(BUILD_BBB)
    add_subdirectory(bbb)
ENDIF()

IF(BUILD_EPC)
    add_subdirectory(epc)
ENDIF()


if(BUILD_BBB AND BUILD_EPC)

    add_custom_command(
        COMMENT "Build C15 update"
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/c15-update.tar
        #DEPENDS epc-create-update
        #DEPENDS bbb-rootfs
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/create-c15-update/create-c15-update.sh
            ${CMAKE_CURRENT_BINARY_DIR}/epc/update.tar
            ${CMAKE_CURRENT_BINARY_DIR}/bbb/nonlinux/output/images/rootfs.tar.gz
            ${CMAKE_CURRENT_BINARY_DIR}
            ${CMAKE_CURRENT_SOURCE_DIR}
    )

    add_custom_target(c15-update DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/c15-update.tar)

ENDIF()
