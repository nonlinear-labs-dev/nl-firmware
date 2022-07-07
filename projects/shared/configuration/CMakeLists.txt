cmake_minimum_required(VERSION 3.2)
project(parameterDB-new)
include(GNUInstallDirs)

add_custom_command(
        COMMENT "create build env"
        OUTPUT .build-env
        DEPENDS Dockerfile
        DEPENDS process.sh
        DEPENDS CMakeLists.txt
        COMMAND podman build -t nl-configuration-build-container ${CMAKE_CURRENT_SOURCE_DIR}
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
        OUTPUT ${PROJECT_BINARY_DIR}/generated
        COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/generated
        COMMAND podman run -v ${PROJECT_SOURCE_DIR}:/sourceDir -v ${PROJECT_BINARY_DIR}:/binaryDir nl-configuration-build-container /sourceDir/process.sh /sourceDir/ /binaryDir/
        COMMAND touch .build-headers
)

add_custom_target(
        configuration
        DEPENDS .build-headers
)