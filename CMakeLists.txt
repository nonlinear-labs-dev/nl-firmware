cmake_minimum_required(VERSION 3.2)

project(C15)

include( CTest )

EXEC_PROGRAM("git" ${CMAKE_CURRENT_SOURCE_DIR} ARGS "log --format=\"C15-%aI--%h\" -n 1" OUTPUT_VARIABLE C15_VERSION)
string(REGEX REPLACE "\:" "-" C15_VERSION ${C15_VERSION})

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    message("This build will use CCache.")
endif()

message("building C15 firmware version: ${C15_VERSION}")

SET(C15_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/${C15_VERSION})

message("Installing into: ${C15_INSTALL_PATH}")

OPTION(BUILD_AUDIOENGINE "Enable building of the audio-engine sub project" ON)
OPTION(BUILD_BBBB "Enable building of the BeagleBoneBlack-Bridge sub project" ON)
OPTION(BUILD_PLAYGROUND "Enable building of the Playground and NonMaps sub projects" ON)
OPTION(BUILD_ONLINEHELP "Enable building and installing of the C15 online help" ON)
OPTION(BUILD_TESTING "Enable tests" ON)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdouble-promotion -Wno-deprecated-declarations")

IF(BUILD_AUDIOENGINE OR BUILD_PLAYGROUND OR BUILD_BBBB)
    add_subdirectory(nltools)
    set(CMAKE_INSTALL_RPATH ${C15_INSTALL_PATH}/nltools)
ENDIF()

IF(BUILD_AUDIOENGINE OR BUILD_PLAYGROUND)
    add_subdirectory(parameter-db)
ENDIF()

IF(BUILD_AUDIOENGINE)
    add_subdirectory(audio-engine)
ENDIF(BUILD_AUDIOENGINE)

IF(BUILD_PLAYGROUND)
    add_subdirectory(playground)
    add_subdirectory(NonMaps)
ENDIF(BUILD_PLAYGROUND)

IF(BUILD_BBBB)
    add_subdirectory(bbbb)
ENDIF(BUILD_BBBB)

IF(BUILD_ONLINEHELP)
    add_subdirectory(online-help)
ENDIF(BUILD_ONLINEHELP)

INSTALL(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ./${C15_VERSION} ${CMAKE_INSTALL_PREFIX}/C15)")
