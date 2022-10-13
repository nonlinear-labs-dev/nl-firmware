cmake_minimum_required(VERSION 3.2)

project(nltools-2)

set(BUILD_SHARED_LIBS ON)

file(GLOB_RECURSE HEADER_FILES ./include/*.h)
file(GLOB_RECURSE SOURCE_FILES ./*.cpp)

add_dependencies(nltools)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wdouble-promotion")
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})

