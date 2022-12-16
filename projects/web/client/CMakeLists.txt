cmake_minimum_required(VERSION 3.0)

add_subdirectory(c16-hwui)

file(GLOB_RECURSE ALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)
configure_file(${CMAKE_CURRENT_SOURCE_DIR}/build-time-info.js.in ${CMAKE_CURRENT_SOURCE_DIR}/build-time-info.js @ONLY)

add_custom_command(
    COMMENT "Web Client"
    OUTPUT bundle.tar.gz
    DEPENDS ${ALL_FILES}
    WORKING_DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}
    COMMAND tar cfz  ${CMAKE_CURRENT_BINARY_DIR}/bundle.tar.gz ./*
    )

add_custom_target(web-client ALL DEPENDS bundle.tar.gz)
