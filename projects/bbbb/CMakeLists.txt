cmake_minimum_required(VERSION 3.0)
project(bbbb)

set(LIBS giomm-2.4 libsoup-2.4)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

IF(NOT DEV_PC)
    file(GLOB_RECURSE UI_SOURCES src/ui/*.cpp)
    list(REMOVE_ITEM SOURCE_FILES EXCLUDE REGEX ${UI_SOURCES})
ENDIF(NOT DEV_PC)

IF(CONTROLS)
    ADD_DEFINITIONS(-D_WINDOW_CONTROLS)
ENDIF(CONTROLS)

IF(DEV_PC)
  ADD_DEFINITIONS(-D_DEVELOPMENT_PC -D_TESTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wmaybe-uninitialized -Wuninitialized -Wdouble-promotion -Wreturn-type")
  set(LIBS ${LIBS} gtkmm-3.0 gdkmm-3.0 libpng)
ENDIF(DEV_PC)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wdouble-promotion")

foreach(lib ${LIBS})
    pkg_check_modules(THE_LIB REQUIRED ${lib})
    target_include_directories(${PROJECT_NAME} PUBLIC ${THE_LIB_INCLUDE_DIRS})
    target_link_libraries(${PROJECT_NAME} PUBLIC ${THE_LIB_LIBRARIES})
    link_directories(${THE_LIB_LIBRARY_DIRS})
endforeach(lib)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_link_libraries(${PROJECT_NAME} PUBLIC nltools)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/../nltools/include/)

configure_file(${PROJECT_SOURCE_DIR}/systemd/${PROJECT_NAME}.service.in ${PROJECT_BINARY_DIR}/systemd/${PROJECT_NAME}.service @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/systemd/${PROJECT_NAME}.service DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR} COMPONENT init)
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})
ADD_CUSTOM_TARGET(touch-bbb-cmakelists ALL COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE})

