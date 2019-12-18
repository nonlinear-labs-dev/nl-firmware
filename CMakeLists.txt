cmake_minimum_required(VERSION 3.2)

project(C15)

EXEC_PROGRAM("git"
  ${CMAKE_CURRENT_SOURCE_DIR}
  ARGS "log -1 --pretty=format:\"%ad\" --date=format:\"%Y-%m-%d %H:%M\""
  OUTPUT_VARIABLE C15_VERSION)

find_program(CCACHE_PROGRAM ccache)
if(CCACHE_PROGRAM)
    set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
endif()

message("building C15 firmware version: ${C15_VERSION}")

EXEC_PROGRAM("sh" ${CMAKE_SOURCE_DIR} ARGS "-c \"git branch | grep '^\\* ' | sed 's/^\\* //'\" " OUTPUT_VARIABLE GIT_BRANCH)
EXEC_PROGRAM("git" ${CMAKE_SOURCE_DIR} ARGS "rev-parse --short HEAD" OUTPUT_VARIABLE GIT_COMMIT)

SET(C15_VERSION C15-${GIT_BRANCH}-${GIT_COMMIT})
SET(C15_INSTALL_PATH ${CMAKE_INSTALL_PREFIX}/${C15_VERSION})

message("Installing into: ${C15_INSTALL_PATH}")

OPTION(BUILD_AUDIOENGINE "Enable building of the audio-engine sub project" ON)
OPTION(BUILD_BBBB "Enable building of the BeagleBoneBlack-Bridge sub project" ON)
OPTION(BUILD_PLAYGROUND "Enable building of the Playground and NonMaps sub projects" ON)
OPTION(BUILD_ONLINEHELP "Enable building and installing of the C15 online help" ON)

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
