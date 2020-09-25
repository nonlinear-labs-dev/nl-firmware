cmake_minimum_required(VERSION 3.0)

project(playcontroller-tools)

# SET
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

# SOURCES
#file(GLOB_RECURSE SOURCE_FILES src/*.c)

# TARGETS
add_executable(playcontroller       src/playcontroller.c        src/linuxgpio.c)
add_executable(playcontroller-read  src/playcontroller-read.c   src/process-read-msgs.c)
add_executable(ehc        src/ehc.c)
add_executable(ehc-preset src/ehc-preset.c  ${CMAKE_SOURCE_DIR}/projects/shared/playcontroller/EHC-pedal-presets.c)


include_directories(src)
include_directories(../)
include_directories(${CMAKE_SOURCE_DIR}/projects)


# INSTALL
install(TARGETS playcontroller          DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS playcontroller-read     DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS ehc                     DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)
install(TARGETS ehc-preset              DESTINATION ${CMAKE_INSTALL_PREFIX}/bin)

install(CODE "execute_process(
    COMMAND bash -c \"
    set -x
    ln -sf ./playcontroller $DESTDIR/bin/plc
    ln -sf ./playcontroller-read $DESTDIR/bin/plc-read
    \")")
