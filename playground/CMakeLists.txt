cmake_minimum_required(VERSION 3.0)
project(playground)
project(playground-tests)

execute_process(COMMAND "git" "rev-parse" "--abbrev-ref" "HEAD"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE git_result
    OUTPUT_VARIABLE BRANCH_NAME)

execute_process(COMMAND "git" "rev-list" "HEAD" "--count"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE git_result
    OUTPUT_VARIABLE COMMIT_COUNT)

execute_process(COMMAND "git" "rev-parse" "--short" "HEAD"
    WORKING_DIRECTORY ${PROJECT_SOURCE_DIR}
    RESULT_VARIABLE git_result
    OUTPUT_VARIABLE HEAD_REF)

string(STRIP ${COMMIT_COUNT} COMMIT_COUNT)
string(STRIP ${HEAD_REF} HEAD_REF)
string(STRIP ${BRANCH_NAME} BRANCH_NAME)

set(LIBS
  gio-unix-2.0
  uuid
  freetype2
  avahi-glib
  avahi-gobject
  avahi-core
  avahi-client
  )

OPTION(DEV_PC
  "Enable some tests and tools not suitable for embedded device"
  OFF)

OPTION(PROFILING
  "Enable built in profiler"
  OFF)

IF(NOT DEFINED SYSTEMD_CONFIGURATION_FILES_DIR)
  set(SYSTEMD_CONFIGURATION_FILES_DIR "${CMAKE_INSTALL_PREFIX}/etc/systemd/system")
ENDIF()

IF(DEV_PC)
  ADD_DEFINITIONS(-D_DEVELOPMENT_PC -D_TESTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wmaybe-uninitialized -Wuninitialized -Wdouble-promotion -Wreturn-type")
  add_subdirectory(src/third-party)
ENDIF(DEV_PC)


IF(PROFILING)
  ADD_DEFINITIONS(-D_PROFILING)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -finstrument-functions -finstrument-functions-exclude-file-list=Profiler.h,Profiler.cpp,map,/c++/")
ENDIF(PROFILING)

function(addLib name)
  pkg_check_modules(${name} REQUIRED ${name})
  include_directories(${${name}_INCLUDE_DIRS})
  link_directories(${${name}_LIBRARY_DIRS})
endfunction(addLib)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)
find_package(Freetype REQUIRED)

foreach(lib ${LIBS})
  addLib(${lib})
endforeach(lib)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdouble-promotion")

configure_file(src/CompileTimeOptions.h.in src/CompileTimeOptions.h)
configure_file(src/device-info/SoftwareVersion.cpp.in src/device-info/SoftwareVersion.cpp)
configure_file(src/proxies/hwui/panel-unit/boled/setup/UISoftwareVersionEditor.cpp.in src/proxies/hwui/panel-unit/boled/setup/UISoftwareVersionEditor.cpp)

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

list(FILTER SOURCE_FILES EXCLUDE REGEX ".*playground-tests.cpp$")

set (EXCLUDE_DIR "/testing/unit-tests/")
foreach (TMP_PATH ${SOURCE_FILES})
  string (FIND ${TMP_PATH} ${EXCLUDE_DIR} EXCLUDE_DIR_FOUND)
  if (NOT ${EXCLUDE_DIR_FOUND} EQUAL -1)
    list (REMOVE_ITEM SOURCE_FILES ${TMP_PATH})
  endif ()
endforeach(TMP_PATH)

file(GLOB_RECURSE TEST_FILES src/*.cpp)
list(FILTER TEST_FILES EXCLUDE REGEX ".*playground.cpp$")

set(SOURCE_FILES
    ${SOURCE_FILES}
)

add_executable(playground ${SOURCE_FILES})
add_executable(playground-test ${TEST_FILES})

target_link_libraries(playground pthread)
target_link_libraries(playground stdc++fs)
target_link_libraries(playground nltools)

target_link_libraries(playground-test pthread)
target_link_libraries(playground-test stdc++fs)
target_link_libraries(playground-test nltools)

include_directories(playground ${CMAKE_SOURCE_DIR}/nltools/include/)
include_directories(playground ${CMAKE_SOURCE_DIR}/playground/src/)
include_directories(playground ${CMAKE_CURRENT_BINARY_DIR}/src/)

include_directories(playground-test ${CMAKE_SOURCE_DIR}/nltools/include/)
include_directories(playground-test ${CMAKE_SOURCE_DIR}/playground/src)

function(linkLib name)
  pkg_check_modules(${name} REQUIRED ${name})
  target_link_libraries(playground ${${name}_LIBRARIES})
  target_link_libraries(playground-test ${${name}_LIBRARIES})
endfunction(linkLib)

foreach(lib ${LIBS})
  linkLib(${lib})
endforeach(lib)

configure_file(${PROJECT_SOURCE_DIR}/systemd/playground.service.in ${PROJECT_BINARY_DIR}/systemd/playground.service @ONLY)

file(GLOB PARAMETER_DESCRIPTIONS src/parameters/descriptions/*.txt)
file(GLOB VERSION_INFOS resources/version-infos/*)

install(FILES ${PROJECT_BINARY_DIR}/systemd/playground.service
    DESTINATION ${SYSTEMD_CONFIGURATION_FILES_DIR} COMPONENT init
)

install(FILES ${PARAMETER_DESCRIPTIONS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/resources/parameter-descriptions
)

install(FILES ${VERSION_INFOS}
    DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground/resources/version-infos
)

install(DIRECTORY resources
    USE_SOURCE_PERMISSIONS
    DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground
)

install(TARGETS playground
    DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground
)

install(TARGETS playground-test
        DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/playground
)

ADD_CUSTOM_TARGET(
    touch-cmakelists ALL
    COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
)


