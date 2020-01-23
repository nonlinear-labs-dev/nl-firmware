OPTION(BUILD_EPC "Build an ePC image" Off)
OPTION(BUILD_BBB "Build an BeagleBoneBlack image" Off)

IF(BUILD_BBB)
    add_subdirectory(bbb)
ENDIF()

IF(BUILD_EPC)
    add_subdirectory(epc)
ENDIF()
