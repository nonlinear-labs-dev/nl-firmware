cmake_minimum_required(VERSION 3.2)
project(parameterDB-new)
include(GNUInstallDirs)

add_custom_command(
        COMMENT "build generated parameter-list from configuration YAML"
        DEPENDS src
        DEPENDS src/definitions
        DEPENDS process.sh
        DEPENDS copy-results-to-source.sh
        OUTPUT ${PROJECT_BINARY_DIR}/generated
        COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/generated
        COMMAND ${PROJECT_SOURCE_DIR}/process.sh ${PROJECT_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${DEV_PC}
        COMMAND touch .build-headers
)

add_custom_target(
        configuration
        DEPENDS ${PROJECT_BINARY_DIR}/generated
)
