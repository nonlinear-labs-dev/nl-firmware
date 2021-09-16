cmake_minimum_required(VERSION 3.0)

project(mc-view)

file(GLOB_RECURSE ALL_FILES *)

configure_file(build-time-info.js build-time-info.js @ONLY)

add_custom_command(
    COMMENT "MC View - staging"
    OUTPUT .staging
    DEPENDS ${ALL_FILES}
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/build-time-info.js
    COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/* ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/build-time-info.js ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND touch .staging
    )

add_custom_command(
    COMMENT "MC View"
    OUTPUT bundle.tar.gz
    DEPENDS .staging
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/staging
    COMMAND tar czf ${CMAKE_CURRENT_BINARY_DIR}/bundle.tar.gz ./*
    )

add_custom_target(mc-view ALL DEPENDS bundle.tar.gz)
