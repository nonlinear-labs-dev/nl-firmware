CMAKE_MINIMUM_REQUIRED(VERSION 3.2)

PROJECT(C15 VERSION 2.0.1)

SET(DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/../nonlinear-labs-downloads" CACHE STRING "Where to store downloads needed for NonlinearLabs-Builds")
SET(NL_SERVER h2949050.stratoserver.net)

INCLUDE(cmake/NL.cmake)
INCLUDE(CTest)


if(NOT TARGET_PLATFORM)
  set(TARGET_PLATFORM "devpc" CACHE STRING "Which target to build for: devpc | epc | epc2 | bbb | playcontroller | pi4")
endif()

SET(NL_SERVER h2949050.stratoserver.net)

MESSAGE("Target platform is: ${TARGET_PLATFORM}")
ADD_DEFINITIONS(-DTARGET_PLATFORM="${TARGET_PLATFORM}")

SET(DOWNLOAD_LOCATION "http://h2949050.stratoserver.net/")

SET(BUILD_CONTAINER_VERSION 2021-10-06) # a timestamp indicating the version of the packages needed to build an update
SET(DOCKERNAME nl-build-container-${BUILD_CONTAINER_VERSION})
SET(METEOR_RESOURCES_REVISION 2022-10-13)
SET(METEOR_PACKAGES kadira:flow-router blaze-html-templates jquery kadira:blaze-layout autoupdate typescript reactive-var meteortesting:mocha less smallhacks:jquery-contextmenu)
SET(NPM_PACKAGES jquery @types/meteor meteor-build-client@0.4.1)
STRING(REPLACE ";" " " METEOR_PACKAGES "${METEOR_PACKAGES}")
STRING(REPLACE ";" " " NPM_PACKAGES "${NPM_PACKAGES}")

set(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
set(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
set(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
set(CPACK_DEBIAN_PACKAGE_DEPENDS "libsoup2.4-1, libflac8, libglibmm-2.4-1v5, libasound2, libfreetype6")

IF (${TARGET_PLATFORM} STREQUAL "pi4")
  set(CPACK_DEBIAN_PACKAGE_ARCHITECTURE "armhf")
ENDIF ()

ADD_SUBDIRECTORY(projects)
ADD_SUBDIRECTORY(build-tools)
ADD_SUBDIRECTORY(os)


SET(CPACK_GENERATOR "DEB")
SET(CPACK_BINARY_DEB On)
SET(CPACK_PACKAGE_CONTACT "info@nonlinear-labs.de")
SET(CPACK_DEBIAN_PACKAGE_MAINTAINER "Nonlinear Labs GmbH")
SET(CPACK_PACKAGE_VERSION_MAJOR ${PROJECT_VERSION_MAJOR})
SET(CPACK_PACKAGE_VERSION_MINOR ${PROJECT_VERSION_MINOR})
SET(CPACK_PACKAGE_VERSION_PATCH ${PROJECT_VERSION_PATCH})
SET(CPACK_DEBIAN_PACKAGE_DEPENDS "libsoup2.4-1, libflac8, libglibmm-2.4-1v5, libasound2, libfreetype6")
SET(CPACK_DEBIAN_PACKAGE_DESCRIPTION "Userland binaries for Nonlinear Labs synthesizers")

message("CPACK: ${CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA}")

INCLUDE(CPack)
INCLUDE(CTest)
