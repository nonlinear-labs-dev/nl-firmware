cmake_minimum_required(VERSION 3.0)
project(nonmaps)

set(GWT_VERSION 2.10.0)

file(GLOB_RECURSE SOURCE_FILES ${CMAKE_CURRENT_SOURCE_DIR}/src/*)

add_custom_command(
  COMMENT "Generate nonmaps-version.txt"
  DEPENDS ${CMAKE_CURRENT_SOURCE_DIR}/nonmaps-version.txt
  DEPENDS ${CMAKE_SOURCE_DIR}/cmake/generate-version.cmake
  DEPENDS ${CMAKE_SOURCE_DIR}/.git/index
  WORKING_DIRECTORY ${CMAKE_SOURCE_DIR}
  COMMAND ${CMAKE_COMMAND}
  -D INPUT_FILE=${CMAKE_CURRENT_SOURCE_DIR}/nonmaps-version.txt
  -D OUTPUT_FILE=${CMAKE_CURRENT_BINARY_DIR}/nonmaps-version.txt
  -P ${CMAKE_SOURCE_DIR}/cmake/generate-version.cmake
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/nonmaps-version.txt
)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/src/main/webapp/NonMaps.js.in ${CMAKE_CURRENT_BINARY_DIR}/NonMaps.js @ONLY)

add_custom_command(
  COMMENT "Download gwt"
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}.zip
  COMMAND wget ${DOWNLOAD_LOCATION}/web/gwt-${GWT_VERSION}.zip -O ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}.zip)

add_custom_command(
  COMMENT "Unpack gwt"
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}/gwt-dev.jar
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}.zip
  COMMAND unzip -uq ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}.zip
  COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}/gwt-dev.jar)

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/war/nonmaps/compilation-mappings.txt
  COMMAND /usr/bin/java -Xmx512m -classpath ${CMAKE_CURRENT_SOURCE_DIR}/src/main/java:${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}/gwt-user.jar:${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}/gwt-dev.jar:${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}/gwt-elemental.jar com.google.gwt.dev.Compiler -optimize 9 -gen ${CMAKE_CURRENT_BINARY_DIR}/obj -logLevel WARN -style OBF -war ./war -sourceLevel 11 -localWorkers 4 com.nonlinearlabs.NonMaps
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/gwt-${GWT_VERSION}/gwt-dev.jar
  DEPENDS ${SOURCE_FILES})

add_custom_command(
  OUTPUT ${CMAKE_CURRENT_BINARY_DIR}/.staging
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/war/nonmaps/compilation-mappings.txt
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/nonmaps-version.txt
  COMMAND rm -rf ${CMAKE_CURRENT_BINARY_DIR}/bundle
  COMMAND mkdir -p ${CMAKE_CURRENT_BINARY_DIR}/bundle
  COMMAND cp -r ${CMAKE_CURRENT_BINARY_DIR}/war/nonmaps/* ${CMAKE_CURRENT_BINARY_DIR}/bundle
  COMMAND cp -r ${CMAKE_CURRENT_SOURCE_DIR}/src/main/webapp/* ${CMAKE_CURRENT_BINARY_DIR}/bundle/
  COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/NonMaps.js ${CMAKE_CURRENT_BINARY_DIR}/bundle/
  COMMAND cp ${CMAKE_CURRENT_BINARY_DIR}/nonmaps-version.txt ${CMAKE_CURRENT_BINARY_DIR}/bundle/
  COMMAND touch ${CMAKE_CURRENT_BINARY_DIR}/.staging
)

add_custom_command(
  OUTPUT bundle.tar.gz
  DEPENDS ${CMAKE_CURRENT_BINARY_DIR}/.staging
  WORKING_DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/bundle/
  COMMAND tar cfz ${CMAKE_CURRENT_BINARY_DIR}/bundle.tar.gz ./*
)

add_custom_target(nonmaps ALL DEPENDS bundle.tar.gz)
