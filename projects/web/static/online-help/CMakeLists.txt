cmake_minimum_required(VERSION 3.0)

project(online-help)

file(GLOB_RECURSE ALL_FILES src/*)

add_custom_command(
    COMMENT "Online Help"
    OUTPUT bundle.tar.gz
    DEPENDS ${ALL_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND tar cfz ${CMAKE_CURRENT_BINARY_DIR}/bundle.tar.gz -C src .
    )

add_custom_target(online-help ALL DEPENDS bundle.tar.gz)
