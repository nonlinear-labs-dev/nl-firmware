project(c16-web)

file(GLOB_RECURSE ALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)
file(GLOB_RECURSE IN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.in)

foreach(IN_FILE ${IN_FILES})
  string(REGEX REPLACE ".in$" "" WITHOUT_IN ${IN_FILE})
  get_filename_component(BASENAME ${WITHOUT_IN} NAME)
  configure_file(${IN_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME} @ONLY)
endforeach()

ADD_CUSTOM_COMMAND(
    COMMENT "Prepare building c16-web"
    OUTPUT .c16-web-prepare
    DEPENDS download-meteor-resources
    COMMAND rm -rf ./meteor-build
    COMMAND mkdir -p ./meteor-build
    COMMAND tar -xzf ${CMAKE_BINARY_DIR}/build-tools/web/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz -C ${CMAKE_CURRENT_BINARY_DIR}/meteor-build
    COMMAND touch .c16-web-prepare
)

ADD_CUSTOM_COMMAND(
    COMMENT "Build c16-web"
    OUTPUT c16-hwui.tar.gz
    DEPENDS ${ALL_FILES}
    DEPENDS .c16-web-prepare
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh ${CMAKE_CURRENT_BINARY_DIR}/build.sh ${CMAKE_CURRENT_SOURCE_DIR} "--network none -v ${CMAKE_CURRENT_BINARY_DIR}/meteor-build:/home/bob-the-builder" 
    COMMAND tar cfz ./c16-hwui.tar.gz -C ${CMAKE_CURRENT_BINARY_DIR}/meteor-build/out ./
)

ADD_CUSTOM_COMMAND(
    COMMENT "Debug c16 hwui"
    OUTPUT .c16-debug-hwui
    DEPENDS ${ALL_FILES}
    DEPENDS .c16-web-prepare
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh ${CMAKE_CURRENT_BINARY_DIR}/debug.sh ${CMAKE_CURRENT_SOURCE_DIR} "-v ${CMAKE_CURRENT_BINARY_DIR}/meteor-build:/home/bob-the-builder -p 3000:3000" 
)

ADD_CUSTOM_TARGET(c16-build-hwui
  DEPENDS epc2-setup-host-os
  DEPENDS c16-hwui.tar.gz
)

ADD_CUSTOM_TARGET(c16-debug-hwui
  DEPENDS epc2-setup-host-os
  DEPENDS .c16-debug-hwui
)
