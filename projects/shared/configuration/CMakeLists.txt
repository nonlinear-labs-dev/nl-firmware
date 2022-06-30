cmake_minimum_required(VERSION 3.2)
project(parameterDB-new)
include(GNUInstallDirs)

EXECUTE_PROCESS(OUTPUT_VARIABLE GROUP_ID COMMAND sh -c "id -g $USER")
STRING(STRIP ${GROUP_ID} GROUP_ID)
EXECUTE_PROCESS(OUTPUT_VARIABLE USER_ID COMMAND sh -c "id -u $USER")
STRING(STRIP ${USER_ID} USER_ID)

add_custom_command(
        COMMENT "create build env"
        OUTPUT .build-env
        DEPENDS Dockerfile
        DEPENDS build-in-docker.sh
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/create-docker.sh ${CMAKE_CURRENT_SOURCE_DIR} ${USER_ID} ${GROUP_ID}
        COMMAND touch .build-env
)

add_custom_command(
        COMMENT "build headers inside build-env"
        DEPENDS .build-env
        DEPENDS src
        DEPENDS copy-results-to-source.sh
        OUTPUT .build-headers
        COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/generated
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build-in-docker.sh ${CMAKE_CURRENT_SOURCE_DIR} ${PROJECT_BINARY_DIR} ${USER_ID} ${GROUP_ID}
        COMMAND touch .build-headers
)

add_custom_target(
        configuration
        ALL
        DEPENDS
        .build-headers
)