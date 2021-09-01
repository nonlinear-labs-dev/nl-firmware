project(web)

configure_file(build-time-info.js ${CMAKE_CURRENT_BINARY_DIR}/build-time-info.js @ONLY)
configure_file(build.sh.in build.sh @ONLY)

add_subdirectory(client)
add_subdirectory(static)

file(GLOB_RECURSE ALL_FILES *.ts *.css *.html *.sh.in *.js)

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
    DEPENDS .prepare-web-build
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static/mc-view/bundle.tar.gz
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static/nonmaps/bundle.tar.gz
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static/online-help/bundle.tar.gz
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/static/recorder/bundle.tar.gz
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh 
              ${CMAKE_CURRENT_BINARY_DIR}/build.sh 
              ${CMAKE_CURRENT_SOURCE_DIR}
              ${CMAKE_CURRENT_BINARY_DIR}
              "--network none" 
    COMMAND touch .web-build
)

ADD_CUSTOM_TARGET(web ALL DEPENDS .web-build)

add_dependencies(web nl-build-container-setup nonmaps recorder mc-view online-help web-client)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/out/ USE_SOURCE_PERMISSIONS DESTINATION ${C15_INSTALL_PATH}/web)

# todo
configure_file(debug.sh.in debug.sh @ONLY)

ADD_CUSTOM_COMMAND(
    COMMENT "Debug web build"
    OUTPUT .debug-web
    DEPENDS ${ALL_FILES}
    DEPENDS .prepare-web-build
    COMMAND ${CMAKE_BINARY_DIR}/build-tools/web/runInWebDocker.sh 
              ${CMAKE_CURRENT_BINARY_DIR}/debug.sh
              ${CMAKE_CURRENT_SOURCE_DIR}
              ${CMAKE_CURRENT_BINARY_DIR}
              "-p 3000:3000" 
)
