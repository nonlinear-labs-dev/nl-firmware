cmake_minimum_required(VERSION 3.0)

project(lpc-tools)

# SET
set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O2 -Wall")

# SOURCES
#file(GLOB_RECURSE SOURCE_FILES src/*.c)

# TARGETS
add_executable(lpc           src/lpc.c         src/linuxgpio.c)
add_executable(lpc-read      src/lpc-read.c    src/process-read-msgs.c)
add_executable(ehc           src/ehc.c)
add_executable(ehc-preset    src/ehc-preset.c  ${CMAKE_SOURCE_DIR}/projects/shared/EHC-pedal-presets.c)
add_executable(make-cal-bin  src/make-cal-bin.c)


include_directories(src)
include_directories(${CMAKE_SOURCE_DIR}/projects/)


# INSTALL
install(TARGETS lpc           DESTINATION /usr/bin)
install(TARGETS lpc-read      DESTINATION /usr/bin)
install(TARGETS ehc           DESTINATION /usr/bin)
install(TARGETS ehc-preset    DESTINATION /usr/bin)
install(TARGETS make-cal-bin  DESTINATION /usr/bin)
