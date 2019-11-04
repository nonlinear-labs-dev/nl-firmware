cmake_minimum_required(VERSION 3.2)

project(parameterDB)

set(BUILD_SHARED_LIBS ON)

include(GNUInstallDirs)

set(sourceDocument ${CMAKE_CURRENT_SOURCE_DIR}/src/ParameterList_1.7B.ods)
set(headerDocument ${CMAKE_CURRENT_SOURCE_DIR}/generated/parameter_list.h)

if(${sourceDocument} IS_NEWER_THAN ${headerDocument})
    message( FATAL_ERROR "${sourceDocument} is newer than the generated header: ${headerDocument}! Please regenerate the headers from the spritesheet!" )
endif()

file(GLOB_RECURSE HEADER_FILES ./generated/*.h)
file(GLOB_RECURSE SOURCE_FILES ./generated/*.cpp)

add_library(parameterDB SHARED ${SOURCE_FILES})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdouble-promotion")

set(headers_install_dir ${CMAKE_INSTALL_INCLUDEDIR}/parameterDB)

install(FILES ${HEADER_FILES} DESTINATION ${headers_install_dir})
install(TARGETS parameterDB DESTINATION ${CMAKE_INSTALL_LIBDIR})

