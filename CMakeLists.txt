cmake_minimum_required(VERSION 3.2)

project(C15)

SET(DOWNLOAD_DIR "${CMAKE_BINARY_DIR}/../nonlinear-labs-downloads" CACHE STRING "Where to store downloads needed for NonlinearLabs-Builds")
SET(NL_SERVER h2949050.stratoserver.net)

include(CTest)
include(cmake/NL.cmake)

#set(TARGET_PLATFORM "devpc" CACHE STRING "Which target to build for: devpc | epc | epc2 | bbb | playcontroller | pi4")

message("Target platform is: ${TARGET_PLATFORM}")
add_definitions(-DTARGET_PLATFORM="${TARGET_PLATFORM}")

SET(DOWNLOAD_LOCATION "http://h2949050.stratoserver.net/")

SET(BUILD_CONTAINER_VERSION 2021-10-06) # a timestamp indicating the version of the packages needed to build an update
SET(DOCKERNAME nl-build-container-${BUILD_CONTAINER_VERSION})
SET(METEOR_RESOURCES_REVISION 2021-08-12)
SET(METEOR_PACKAGES kadira:flow-router blaze-html-templates jquery kadira:blaze-layout autoupdate typescript reactive-var)
SET(NPM_PACKAGES jquery @types/meteor meteor-build-client@0.4.1)
string(REPLACE ";" " " METEOR_PACKAGES "${METEOR_PACKAGES}")
string(REPLACE ";" " " NPM_PACKAGES "${NPM_PACKAGES}")

add_subdirectory(os)
add_subdirectory(projects)
add_subdirectory(build-tools)

