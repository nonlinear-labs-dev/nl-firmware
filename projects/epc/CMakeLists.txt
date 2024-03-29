OPTION(BUILD_ACCEPTANCE_TESTS "Build the acceptance tests for audio-engine and playground" Off)
OPTION(BUILD_AUDIOENGINE "Build the audio-engine" Off)
OPTION(BUILD_PLAYGROUND "Build the playground" Off)
OPTION(BUILD_EPC_SCRIPTS "Install the shell scripts on the ePC" Off)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdouble-promotion")

IF(${TARGET_PLATFORM} STREQUAL "epc" OR ${TARGET_PLATFORM} STREQUAL "epc2")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=broadwell")
ELSEIF (${TARGET_PLATFORM} STREQUAL "pi4" )
    MESSAGE("building for Pi4")
ELSE ()
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=native")
ENDIF()

IF(BUILD_ACCEPTANCE_TESTS)
    add_subdirectory(acceptance-tests)
ENDIF()

IF(BUILD_AUDIOENGINE)
    add_subdirectory(audio-engine)
ENDIF()

IF(BUILD_PLAYGROUND)
    add_subdirectory(playground)
ENDIF()

IF(BUILD_EPC_SCRIPTS)
    add_subdirectory(scripts)
ENDIF()
