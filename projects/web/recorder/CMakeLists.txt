cmake_minimum_required(VERSION 3.0)

project(recorder)

file(GLOB_RECURSE TS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.ts)
file(GLOB_RECURSE ALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/constants.ts.in ${CMAKE_CURRENT_BINARY_DIR}/constants.ts)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/tsconfig.json ${CMAKE_CURRENT_BINARY_DIR}/tsconfig.json)

add_custom_command(
    COMMENT "Recorder"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/recorder.js
    DEPENDS ${ALL_FILES}
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/build.sh 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
              "--network none")

add_custom_target(recorder ALL SOURCES ${CMAKE_CURRENT_BINARY_DIR}/recorder.js)
