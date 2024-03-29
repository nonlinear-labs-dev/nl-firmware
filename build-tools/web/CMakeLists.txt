project(web)

file(GLOB_RECURSE ALL_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*)
file(GLOB_RECURSE IN_FILES ${CMAKE_CURRENT_SOURCE_DIR}/*.in)

foreach(IN_FILE ${IN_FILES})
  string(REGEX REPLACE ".in$" "" WITHOUT_IN ${IN_FILE})
  get_filename_component(BASENAME ${WITHOUT_IN} NAME)
  configure_file(${IN_FILE} ${CMAKE_CURRENT_BINARY_DIR}/${BASENAME} @ONLY)
endforeach()

ADD_CUSTOM_COMMAND(
    COMMENT "Fetch up-to-date meteor resources"
    OUTPUT fetched/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz
    COMMAND ${CMAKE_CURRENT_BINARY_DIR}/runInWebDocker.sh
      ${CMAKE_CURRENT_BINARY_DIR}/fetchMeteorResources.sh 
      ${CMAKE_CURRENT_SOURCE_DIR} 
      ${CMAKE_CURRENT_BINARY_DIR}
)

ADD_CUSTOM_COMMAND(
    COMMENT "Download Meteor packages for ${METEOR_RESOURCES_REVISION}"
    OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz
    COMMAND wget ${DOWNLOAD_LOCATION}/web/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz)

ADD_CUSTOM_TARGET(fetch-meteor-resources
  DEPENDS nl-build-container-setup
  DEPENDS fetched/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz
)

ADD_CUSTOM_TARGET(download-meteor-resources
    DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/meteor-resources-${METEOR_RESOURCES_REVISION}.tar.gz)
  
ADD_CUSTOM_TARGET(web-package DEPENDS web.deb)

add_dependencies(download-meteor-resources nl-build-container-setup)
