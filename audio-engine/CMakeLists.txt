cmake_minimum_required(VERSION 3.0)
project(audio-engine)

set(LIBS
  glibmm-2.4
  giomm-2.4
  glib-2.0
  gio-2.0
  alsa
  )

IF(CLANG)
    SET (CMAKE_C_COMPILER             "/usr/bin/clang")
    SET (CMAKE_C_FLAGS                "-Wall -std=c99")
    SET (CMAKE_C_FLAGS_DEBUG          "-g")
    SET (CMAKE_C_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELEASE        "-O4 -DNDEBUG")
    SET (CMAKE_C_FLAGS_RELWITHDEBINFO "-O2 -g")

    SET (CMAKE_CXX_COMPILER             "/usr/bin/clang++")
    SET (CMAKE_CXX_FLAGS                "-Wall")
    SET (CMAKE_CXX_FLAGS_DEBUG          "-g")
    SET (CMAKE_CXX_FLAGS_MINSIZEREL     "-Os -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELEASE        "-O4 -DNDEBUG")
    SET (CMAKE_CXX_FLAGS_RELWITHDEBINFO "-O2 -g")
ENDIF(CLANG)

find_package(OpenMP)

IF(OPENMP_FOUND)
message ("openMP")
    set (CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${OpenMP_C_FLAGS}")
    set (CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
    set (CMAKE_EXE_LINKER_FLAGS "${CMAKE_EXE_LINKER_FLAGS} ${OpenMP_EXE_LINKER_FLAGS}")
ELSE()
    message ("No openMP")
ENDIF()

function(addLib name)
  pkg_check_modules(${name} REQUIRED ${name})
  include_directories(${${name}_INCLUDE_DIRS})
  link_directories(${${name}_LIBRARY_DIRS})
endfunction(addLib)

find_package(PkgConfig REQUIRED)
find_package(Threads REQUIRED)

foreach(lib ${LIBS})
  addLib(${lib})
endforeach(lib)

include_directories(src)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdouble-promotion -march=broadwell -Ofast")

file(GLOB_RECURSE SOURCE_FILES src/*.cpp)

add_executable(audio-engine ${SOURCE_FILES})

IF(DEV_PC)
    TARGET_COMPILE_DEFINITIONS(audio-engine PRIVATE test_inputModeFlag=1)
ENDIF(DEV_PC)

TARGET_LINK_LIBRARIES(audio-engine pthread)

function(linkLib name)
  pkg_check_modules(${name} REQUIRED ${name})
  target_link_libraries(audio-engine ${${name}_LIBRARIES})
endfunction(linkLib)

foreach(lib ${LIBS})
  linkLib(${lib})
endforeach(lib)

install(TARGETS audio-engine
  DESTINATION ${CMAKE_INSTALL_PREFIX}/nonlinear/audio-engine
  )

ADD_CUSTOM_TARGET(
	audio-engine-touch-cmakelists ALL
	COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
)




