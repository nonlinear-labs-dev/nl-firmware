cmake_minimum_required(VERSION 3.2)

project(nltools2)

set(BUILD_SHARED_LIBS ON)

file(GLOB_RECURSE HEADER_FILES ./include/*.h)
file(GLOB_RECURSE SOURCE_FILES ./*.cpp)

add_library(${PROJECT_NAME} SHARED ${SOURCE_FILES})

add_dependencies(${PROJECT_NAME} configuration)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_BINARY_DIR}/projects/shared/configuration/generated/)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/include/)

prepare_nltools()
add_nltools(${PROJECT_NAME})

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wdouble-promotion")
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})
