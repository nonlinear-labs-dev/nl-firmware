cmake_minimum_required(VERSION 3.2)
project(parameterDB-new)
include(GNUInstallDirs)

add_custom_command(
        COMMENT "create build env"
        OUTPUT .build-env
        DEPENDS Dockerfile
        DEPENDS build-in-docker.sh
        DEPENDS process.sh
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/create-docker.sh ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND touch .build-env
)

add_custom_command(
        COMMENT "build headers inside build-env"
        DEPENDS .build-env
        DEPENDS src
        DEPENDS src/definitions
        DEPENDS process.sh
        DEPENDS copy-results-to-source.sh
        OUTPUT .build-headers
        COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/generated
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build-in-docker.sh ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR}
        COMMAND touch .build-headers
)

add_custom_target(
        configuration
        ALL
        DEPENDS
        .build-headers
)