cmake_minimum_required(VERSION 3.0)
project(C15-scripts)

file(GLOB_RECURSE SCRIPTS ${CMAKE_CURRENT_SOURCE_DIR}/*.sh)
install(FILES ${SCRIPTS} DESTINATION ${C15_INSTALL_PATH}/scripts)

