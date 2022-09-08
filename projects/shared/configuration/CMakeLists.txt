cmake_minimum_required(VERSION 3.2)
project(parameterDB-new)
include(GNUInstallDirs)

file(GLOB_RECURSE YAML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.yaml)
file(GLOB_RECURSE IN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.in)

add_custom_command(
        COMMENT "build generated parameter-list from configuration YAML"
        DEPENDS ${YAML_FILES}
        DEPENDS ${IN_FILES}
        DEPENDS process.sh
        DEPENDS copy-results-to-source.sh
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/.done
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/process.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${DEV_PC}
        COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/generated/.done
)

add_custom_target(
        configuration
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated/.done
)
