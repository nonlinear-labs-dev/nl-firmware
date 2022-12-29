project(web)

configure_file(build.sh.in build.sh @ONLY)

add_subdirectory(client)
add_subdirectory(static)
add_subdirectory(public)

file(GLOB_RECURSE ALL_FILES *.ts *.css *.html *.sh.in *.js)
list(FILTER ALL_FILES EXCLUDE REGEX /web/public/)
list(FILTER ALL_FILES EXCLUDE REGEX /web/.meteor/local/)
list(FILTER ALL_FILES EXCLUDE REGEX /nonmaps/target/)

file(GLOB_RECURSE NONMAPS_TARGET ${CMAKE_SOURCE_DIR}/projects/web/static//nonmaps/target/*)
list(REMOVE_ITEM ALL_FILES_IN_PROJECT EXCLUDE REGEX ${NONMAPS_TARGET})

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
    OUTPUT .do-web-build
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
              "--network none --rm" 
    COMMAND touch .do-web-build
)

ADD_CUSTOM_COMMAND(
    COMMENT "Package web build"
    OUTPUT web.tar.gz
    DEPENDS .do-web-build
    WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/out
    COMMAND tar cfz ${CMAKE_CURRENT_BINARY_DIR}/web.tar.gz ./*
)

ADD_CUSTOM_TARGET(web ALL DEPENDS web-public web.tar.gz)

add_dependencies(web nl-build-container-setup nonmaps recorder mc-view online-help)
install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/out/ USE_SOURCE_PERMISSIONS DESTINATION ${C15_INSTALL_PATH}/web)
