cmake_minimum_required(VERSION 3.2)

project(nltools)

add_subdirectory(tests)

set(BUILD_SHARED_LIBS ON)

file(GLOB_RECURSE HEADER_FILES ./include/*.h)
file(GLOB_RECURSE SOURCE_FILES ./*.cpp)
file(GLOB_RECURSE TEST_FILES ./tests/*)

list(REMOVE_ITEM SOURCE_FILES EXCLUDE REGEX ${TEST_FILES})

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})
find_package(PkgConfig REQUIRED)

pkg_check_modules(MOD REQUIRED glibmm-2.4 giomm-2.4 libsoup-2.4)
target_include_directories(${PROJECT_NAME} PUBLIC ${MOD_INCLUDE_DIRS})
target_link_libraries(${PROJECT_NAME} PUBLIC ${MOD_LIBRARIES})

if(${CMAKE_VERSION} VERSION_LESS "3.13.0")
    link_directories(${PROJECT_NAME} PUBLIC ${MOD_LIBRARY_DIRS})
else()
    target_link_directories(${PROJECT_NAME} PUBLIC ${MOD_LIBRARY_DIRS})
endif()

target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src/)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wdouble-promotion")
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})

add_dependencies(${PROJECT_NAME} configuration)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/projects/shared/configuration/generated)