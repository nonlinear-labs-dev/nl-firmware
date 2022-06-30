cmake_minimum_required(VERSION 3.2)

project(parameterDB-new)

include(GNUInstallDirs)

set(sourceDocument ${CMAKE_CURRENT_SOURCE_DIR}/src/ParameterList_1.7B.ods)
set(headerDocument ${CMAKE_CURRENT_SOURCE_DIR}/generated/parameter_list.h)

USER_ID=$(id -u)

add_custom_command(
        COMMENT "create build env"
        OUTPUT .build-env
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/create-docker.sh ${CMAKE_CURRENT_SOURCE_DIR}
        COMMAND touch .build-env
)

add_custom_command(
        COMMENT "build headers inside build-env"
        OUTPUT ./generated
        OUTPUT .build-headers
        COMMAND bash ${CMAKE_CURRENT_SOURCE_DIR}/build-in-docker.sh ${CMAKE_CURRENT_SOURCE_DIR} ${CMAKE_CURRENT_BINARY_DIR} ${CMAKE_SOURCE_DIR}
        COMMAND touch .build-headers
)

add_custom_target(
        configuration
        ALL
        DEPENDS
        .build-env
        .build-headers
)

#nl-configuration-build-container

message(${sourceDocument})
message(${CMAKE_CURRENT_SOURCE_DIR}/generated)
#install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/generated/ USE_SOURCE_PERMISSIONS DESTINATION ${C15_INSTALL_PATH}/web)
