OPTION(BUILD_PLAYCONTROLLER "Build the playcontroller firmware" Off)
OPTION(BUILD_PLAYCONTROLLER_TOOLS "Build the BBB tools for playcontroller" Off)

IF(BUILD_PLAYCONTROLLER)
    add_subdirectory(firmware)
ENDIF()

IF(BUILD_PLAYCONTROLLER_TOOLS)
    add_subdirectory(tools)
ENDIF()
