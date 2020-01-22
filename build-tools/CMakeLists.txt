OPTION(BUILD_EPC "Build an ePC image" Off)
OPTION(BUILD_BBB "Build an BeagleBoneBlack image" Off)
OPTION(BUILD_DEV_PC "Build executables to be run on a developer PC" Off)

IF(BUILD_BBB)
    add_subdirectory(bbb)
ENDIF()