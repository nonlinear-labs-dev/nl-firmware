cmake_minimum_required(VERSION 3.0)

project(recorder)

file(GLOB_RECURSE ALL_FILES *)

configure_file(constants.ts.in constants.ts)
configure_file(tsconfig.json tsconfig.json)

add_custom_command(
    COMMENT "Recorder"
    OUTPUT bundle.tar.gz
    DEPENDS ${ALL_FILES}
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh
            ${CMAKE_CURRENT_SOURCE_DIR}/build.sh 
            ${CMAKE_CURRENT_SOURCE_DIR}
            ${CMAKE_CURRENT_BINARY_DIR}
              "--network none")

add_custom_target(recorder ALL SOURCES bundle.tar.gz)
