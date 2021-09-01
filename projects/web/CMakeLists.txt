project(web)

configure_file(build-time-info.js ${CMAKE_CURRENT_BINARY_DIR}/build-time-info.js @ONLY)
configure_file(build.sh.in build.sh @ONLY)
configure_file(debug.sh.in debug.sh @ONLY)

file(GLOB_RECURSE ALL_FILES *.ts *.css *.html *.sh.in)

add_subdirectory(nonmaps)
add_subdirectory(recorder)

ADD_CUSTOM_COMMAND(
    COMMENT "Prepare building for web"
    OUTPUT .prepare-web-build
    DEPENDS download-meteor-resources
    COMMAND rm -rf ./meteor-build
    COMMAND mkdir -p ./meteor-build
    COMMAND tar -xzf ${CMAKE_BINARY_DIR}/build-tools/web/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz 
            -C ${CMAKE_CURRENT_BINARY_DIR}
    COMMAND touch .prepare-web-build
)

ADD_CUSTOM_COMMAND(
    COMMENT "Build for web"
    OUTPUT .web-build
    DEPENDS ${ALL_FILES}
    DEPENDS nonmaps
    DEPENDS c16-hwui
    DEPENDS recorder
    DEPENDS nl-build-container-setup
    DEPENDS .prepare-web-build
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh 
              ${CMAKE_CURRENT_BINARY_DIR}/build.sh 
              ${CMAKE_CURRENT_SOURCE_DIR}
              ${CMAKE_CURRENT_BINARY_DIR}
              "--network none" 
)

ADD_CUSTOM_COMMAND(
    COMMENT "Debug web build"
    OUTPUT .debug-web
    DEPENDS ${ALL_FILES}
    DEPENDS nl-build-container-setup
    DEPENDS .prepare-web-build
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh 
              ${CMAKE_CURRENT_BINARY_DIR}/debug.sh
              ${CMAKE_CURRENT_SOURCE_DIR}
              ${CMAKE_CURRENT_BINARY_DIR}
              "-p 3000:3000" 
)

ADD_CUSTOM_TARGET(web
    DEPENDS .web-build
)

ADD_CUSTOM_TARGET(debug-web
  DEPENDS .debug-web
)

# install mc-view
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/mc-view DESTINATION ${C15_INSTALL_PATH}/web)

# install recorder
install(FILES ${CMAKE_CURRENT_BINARY_DIR}/recorder/recorder.js DESTINATION ${C15_INSTALL_PATH}/web/recorder)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/recorder/index.html DESTINATION ${C15_INSTALL_PATH}/web/recorder)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/recorder/styles.css DESTINATION ${C15_INSTALL_PATH}/web/recorder)
install(FILES ${CMAKE_CURRENT_SOURCE_DIR}/recorder/favicon.ico DESTINATION ${C15_INSTALL_PATH}/web/recorder)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/recorder/fonts DESTINATION ${C15_INSTALL_PATH}/web/recorder)
install(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR}/recorder/img DESTINATION ${C15_INSTALL_PATH}/web/recorder)
