cmake_minimum_required(VERSION 3.0)
project(bbbb)

set(LIBS
  glibmm-2.4
  giomm-2.4
  glib-2.0
  gio-2.0
  gio-unix-2.0
  gtkmm-3.0
  libsoup-2.4
)

IF(DEV_PC)
  ADD_DEFINITIONS(-D_DEVELOPMENT_PC -D_TESTS)
  set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wmaybe-uninitialized -Wuninitialized -Wdouble-promotion -Wreturn-type")
ENDIF(DEV_PC)

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

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -Wdouble-promotion")
  
file(GLOB_RECURSE SOURCE_FILES src/*.cpp) 
  
add_executable(bbbb ${SOURCE_FILES})

TARGET_LINK_LIBRARIES(bbbb pthread boost_system)

function(linkLib name)
  pkg_check_modules(${name} REQUIRED ${name})
  target_link_libraries(bbbb ${${name}_LIBRARIES})
endfunction(linkLib)

foreach(lib ${LIBS})
  linkLib(${lib})
endforeach(lib)

install(TARGETS bbbb
  DESTINATION ./
  )

ADD_CUSTOM_TARGET(
	touch-bbb-cmakelists ALL
	COMMAND ${CMAKE_COMMAND} -E touch ${CMAKE_CURRENT_LIST_FILE}
)

