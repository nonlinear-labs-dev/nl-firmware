cmake_minimum_required(VERSION 3.0)

project(web-core)

file(GLOB_RECURSE ALL_FILES *)

add_custom_command(
    COMMENT "Web Core"
    OUTPUT bundle.tar.gz
    DEPENDS ${ALL_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND tar cfzv ${CMAKE_CURRENT_BINARY_DIR}/bundle.tar.gz ./*
    )

add_custom_target(web-core ALL SOURCES bundle.tar.gz)
