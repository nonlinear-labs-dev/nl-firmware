cmake_minimum_required(VERSION 3.2)
project(parameterDB-new)
include(GNUInstallDirs)

file(GLOB_RECURSE YAML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.yaml)
file(GLOB_RECURSE IN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.in)
file(GLOB_RECURSE TYPESCRIPT_FILES ${CMAKE_CURRENT_SOURCE_DIR}/lib/*.ts)
file(GLOB_RECURSE SHELL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.sh)

add_custom_command(
        COMMENT "build generated parameter-list from configuration YAML"
        DEPENDS ${YAML_FILES}
        DEPENDS ${IN_FILES}
        DEPENDS ${TYPESCRIPT_FILES}
        DEPENDS ${SHELL_FILES}
        DEPENDS Dockerfile
        DEPENDS package.json
        DEPENDS tsconfig.json
        OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/generated/.done
        COMMAND ${CMAKE_CURRENT_SOURCE_DIR}/build-in-docker.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${DEV_PC}
        COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/generated/.done
)

add_custom_target(
        configuration
        DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/generated/.done
)
