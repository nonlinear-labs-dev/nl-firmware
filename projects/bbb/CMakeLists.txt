OPTION(BUILD_BRIDGE "Build the BeagleBoneBlack Bridge" Off)
OPTION(BUILD_ESPI_DRIVER "Build the BeagleBoneBlack espi driver" Off)
OPTION(BUILD_PLAYCONTROLLER_DRIVER "Build the BeagleBoneBlack playcontroller driver" Off)
OPTION(BUILD_TEXT2SOLED "Build the text2soled tool" Off)
OPTION(BUILD_BBB_SCRIPTS "Install the shell scripts on the BeagleBoneBlack" Off)

IF(BUILD_BRIDGE)
    add_subdirectory(bridge)
ENDIF()

IF(BUILD_ESPI_DRIVER)
    add_subdirectory(espi-driver)
ENDIF()

IF(BUILD_PLAYCONTROLLER_DRIVER)
    add_subdirectory(playcontroller-driver)
ENDIF()

IF(BUILD_TEXT2SOLED)
    add_subdirectory(text2soled)
ENDIF()

IF(BUILD_BBB_SCRIPTS)
    add_subdirectory(scripts)
ENDIF()
