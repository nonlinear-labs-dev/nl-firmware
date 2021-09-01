cmake_minimum_required(VERSION 3.0)

project(c16-hwui)

file(GLOB_RECURSE ALL_FILES *)

add_custom_command(
    COMMENT "C16 HWUI"
    OUTPUT bundle.tar.gz
    DEPENDS ${ALL_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND tar cfz ${CMAKE_CURRENT_BINARY_DIR}/bundle.tar.gz ./*
    )

add_custom_target(c16-hwui ALL SOURCES bundle.tar.gz)
