cmake_minimum_required(VERSION 3.0)

project(lpc)

# SET
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

# SOURCES
file(GLOB_RECURSE SOURCE_FILES src/*.c)

# TARGETS
add_executable(${PROJECT_NAME} ${SOURCE_FILES})

include_directories(src)

# INSTALL
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})
