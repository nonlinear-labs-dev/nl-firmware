set(CMAKE_SYSTEM_NAME Linux)
set(CMAKE_SYSTEM_PROCESSOR armv8-a)

set(CMAKE_SYSROOT /)
set(CMAKE_STAGING_PREFIX /staging)
set(CMAKE_FIND_ROOT_PATH /usr/lib/arm-linux-gnueabihf)

set(CMAKE_C_COMPILER /usr/bin/arm-linux-gnueabihf-gcc)
set(CMAKE_CXX_COMPILER /usr/bin/arm-linux-gnueabihf-g++)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -march=armv8-a -Wno-psabi -mfpu=neon-fp-armv8")

set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_PACKAGE ONLY)

SET(TARGET_PLATFORM "pi4")

INCLUDE_DIRECTORIES(SYSTEM /usr/include)

