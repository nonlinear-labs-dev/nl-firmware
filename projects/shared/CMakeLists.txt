
IF(BUILD_PLAYGROUND OR BUILD_AUDIOENGINE OR BUILD_BRIDGE OR BUILD_ACCEPTANCE_TESTS)
    add_subdirectory(nltools)
ENDIF()

IF(BUILD_PLAYGROUND OR BUILD_AUDIOENGINE)
    add_subdirectory(nltools-2)
ENDIF()

add_subdirectory(i18n)
add_subdirectory(playcontroller)
add_subdirectory(third-party)
add_subdirectory(configuration)