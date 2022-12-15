

FUNCTION(add_static_web_project_to_meteor NAME)
  ADD_CUSTOM_COMMAND(COMMENT "Prepare ${NAME} for usage in meteor project"
    OUTPUT ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}/.built
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/../static/${NAME}/bundle.tar.gz
    COMMAND rm -rf ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}
    COMMAND mkdir -p ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}
    COMMAND tar -C ${CMAKE_CURRENT_SOURCE_DIR}/${NAME} -xf ${CMAKE_CURRENT_BINARY_DIR}/../static/${NAME}/bundle.tar.gz
    COMMAND touch ${CMAKE_CURRENT_SOURCE_DIR}/${NAME}/.built)
ENDFUNCTION()

add_static_web_project_to_meteor (mc-view)
add_static_web_project_to_meteor (nonmaps)
add_static_web_project_to_meteor (online-help)
add_static_web_project_to_meteor (recorder)

ADD_CUSTOM_TARGET(web-public DEPENDS mc-view nonmaps online-help recorder mc-view/.built nonmaps/.built online-help/.built recorder/.built)
