cmake_minimum_required(VERSION 3.0)

project(text2soled)

# PRECONDITIONS
find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(Freetype REQUIRED)

# SET
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wdouble-promotion")
set(LIBS giomm-2.4 freetype2)

# SOURCES
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
file(GLOB RESOURCES resources/*)

# TARGETS
add_executable(${PROJECT_NAME} ${SOURCE_FILES})


# LIBS
foreach(lib ${LIBS})
  pkg_check_modules(THE_LIB REQUIRED ${lib})
  target_include_directories(${PROJECT_NAME} PUBLIC ${THE_LIB_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${THE_LIB_LIBRARIES})
  link_directories(${THE_LIB_LIBRARY_DIRS})
endforeach()

target_link_libraries(${PROJECT_NAME} PUBLIC pthread)

include_directories(src)

# INSTALL
install(FILES ${RESOURCES} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME}/resources)
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})

