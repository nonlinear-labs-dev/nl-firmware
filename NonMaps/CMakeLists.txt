cmake_minimum_required(VERSION 3.0)
project(NonMaps)

message("searching for gwt compiler in: ${JARSDIR}")

set(NONMAPS_ROOT ${CMAKE_CURRENT_SOURCE_DIR})
set(OBJDIR ${CMAKE_CURRENT_BINARY_DIR}/obj)
set(ONLINEHELPDIR ${NONMAPS_ROOT}/online-help)
set(OUTFILE ${CMAKE_CURRENT_BINARY_DIR}/war/nonmaps/compilation-mappings.txt)

file(GLOB_RECURSE SOURCE_FILES
  ${NONMAPS_ROOT}/src/*.java)

file (GLOB MCSITE ${NONMAPS_ROOT}/PerformanceView/*)

add_custom_command(OUTPUT ${OUTFILE}
  COMMAND /usr/bin/java -Xmx512m -classpath ${NONMAPS_ROOT}/src/main/java:${JARSDIR}/gwt-user.jar:${JARSDIR}/gwt-dev.jar com.google.gwt.dev.Compiler -optimize 9 -gen ${OBJDIR} -logLevel WARN -style OBF -war ./war -localWorkers 4 com.nonlinearlabs.NonMaps
  DEPENDS ${SOURCE_FILES}
  )

add_custom_target(NonMaps ALL
  SOURCES ${OUTFILE}
  )

file(WRITE ${OBJDIR}/nonmaps-version.txt ${PLAYGROUND_VERSION})

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/war
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/NonMaps
  )

install(DIRECTORY ${CMAKE_CURRENT_BINARY_DIR}/war/nonmaps
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/NonMaps/war
  )

  install(DIRECTORY ${NONMAPS_ROOT}/src/main/webapp/
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/NonMaps/war
  )

install(DIRECTORY ${ONLINEHELPDIR}
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/NonMaps/war
  )

install(DIRECTORY MCView
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/NonMaps
  )

install(FILES ${OBJDIR}/nonmaps-version.txt
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/NonMaps/war
  )


