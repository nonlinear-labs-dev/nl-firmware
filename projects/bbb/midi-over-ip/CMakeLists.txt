cmake_minimum_required(VERSION 3.0)
project(midi-over-ip)

enable_ccache()
find_systemd()
prepare_nltools()

set(LIBS alsa)
file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(${PROJECT_NAME} ${SOURCE_FILES})
add_nltools(${PROJECT_NAME})

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++14 -Wdouble-promotion")

foreach(lib ${LIBS})
  pkg_check_modules(${lib} REQUIRED ${lib})
  target_include_directories(${PROJECT_NAME} PUBLIC ${${lib}_INCLUDE_DIRS})
  target_link_libraries(${PROJECT_NAME} PUBLIC ${${lib}_LIBRARIES})
  link_directories(${${lib}_LIBRARY_DIRS})
endforeach(lib)

target_link_libraries(${PROJECT_NAME} PUBLIC pthread)
target_include_directories(${PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/src)

configure_file(${PROJECT_SOURCE_DIR}/systemd/midi-over-ip.service.in ${PROJECT_BINARY_DIR}/systemd/midi-over-ip.service @ONLY)
install(FILES ${PROJECT_BINARY_DIR}/systemd/midi-over-ip.service DESTINATION ${SYSTEMD_SERVICES_INSTALL_DIR} COMPONENT init)
install(TARGETS ${PROJECT_NAME} DESTINATION ${C15_INSTALL_PATH}/${PROJECT_NAME})

install(CODE "execute_process(
    COMMAND bash -c \"set -e
    mkdir -p $DESTDIR/${SYSTEMD_SERVICES_INSTALL_DIR}/multi-user.target.wants
    mkdir -p $DESTDIR/${SYSTEMD_SERVICES_INSTALL_DIR}/sound.target.wants
    mkdir -p $DESTDIR/${SYSTEMD_SERVICES_INSTALL_DIR}/sysinit.target.wants
    ln -sf ../midi-over-ip.service $DESTDIR/${SYSTEMD_SERVICES_INSTALL_DIR}/sound.target.wants/midi-over-ip.service
    \")")

