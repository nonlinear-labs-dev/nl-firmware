cmake_minimum_required(VERSION 3.0)
project(audio-engine)

SET(AE_LIB_NAME ${PROJECT_NAME}-lib)

find_systemd()
enable_ccache()
prepare_nltools_2()

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)
list(REMOVE_ITEM SOURCE_FILES EXCLUDE ${CMAKE_CURRENT_SOURCE_DIR}/src/main.cpp)

add_library(${AE_LIB_NAME} STATIC ${SOURCE_FILES})
add_nltools_2(${AE_LIB_NAME})

add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${AE_LIB_NAME})

set(LIBS alsa flac)

find_package(OpenMP)

IF(OPENMP_FOUND)
    set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set(CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ENDIF()

if(${CMAKE_SYSTEM_PROCESSOR} MATCHES "armv7")
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -mfpu=neon-fp-armv8")
endif()

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

foreach(lib ${LIBS})
    pkg_check_modules(${lib} REQUIRED ${lib})
    target_include_directories(${AE_LIB_NAME} INTERFACE ${${lib}_INCLUDE_DIRS})
    target_link_libraries(${AE_LIB_NAME} INTERFACE ${${lib}_LIBRARIES})
    link_directories(${${lib}_LIBRARY_DIRS})
endforeach(lib)

include_directories(src)

IF(DEV_PC)
    TARGET_COMPILE_DEFINITIONS(${AE_LIB_NAME} PRIVATE test_inputModeFlag=1)
ENDIF(DEV_PC)

add_dependencies(${AE_LIB_NAME} configuration)

target_link_libraries(${AE_LIB_NAME} PUBLIC pthread)
target_include_directories(${AE_LIB_NAME} PUBLIC ${CMAKE_BINARY_DIR}/projects/shared/configuration/generated/)
target_include_directories(${AE_LIB_NAME} PUBLIC ${CMAKE_SOURCE_DIR}/projects/shared)

add_subdirectory(tests)

install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})

IF (${TARGET_PLATFORM} STREQUAL "pi4")
  configure_file(${PROJECT_SOURCE_DIR}/system/${PROJECT_NAME}.service.pi4.in ${PROJECT_BINARY_DIR}/system/${PROJECT_NAME}.service @ONLY)
ELSE()
  configure_file(${PROJECT_SOURCE_DIR}/system/${PROJECT_NAME}.service.epc.in ${PROJECT_BINARY_DIR}/system/${PROJECT_NAME}.service @ONLY)
ENDIF()

configure_file(${PROJECT_SOURCE_DIR}/system/fixCpuBindings.service.in ${PROJECT_BINARY_DIR}/system/fixCpuBindings.service @ONLY)

install(FILES ${PROJECT_SOURCE_DIR}/system/asound.state DESTINATION ${C15_ROOT_PATH}/var/lib/alsa/)
install(FILES ${PROJECT_BINARY_DIR}/system/${PROJECT_NAME}.service DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR} COMPONENT init)
install(FILES ${PROJECT_BINARY_DIR}/system/fixCpuBindings.service DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR} COMPONENT init)

install(DIRECTORY DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR}/multi-user.target.wants)
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ../fixCpuBindings.service $ENV{DESTDIR}/${SYSTEMD_SERVICES_INSTALL_DIR}/multi-user.target.wants/fixCpuBindings.service)")
install(CODE "execute_process(COMMAND ${CMAKE_COMMAND} -E create_symlink ../audio-engine.service $ENV{DESTDIR}/${SYSTEMD_SERVICES_INSTALL_DIR}/multi-user.target.wants/audio-engine.service)")