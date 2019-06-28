include(GNUInstallDirs)

file(GLOB_RECURSE HEADER_FILES ./*.h)
file(GLOB_RECURSE SOURCE_FILES ./*.cpp)

set(LIBS
  glibmm-2.4
  giomm-2.4
  )

add_library(nltools SHARED ${SOURCE_FILES})
find_package(PkgConfig REQUIRED)

function(addLib name)
  pkg_check_modules(${name} REQUIRED ${name})
  include_directories(${${name}_INCLUDE_DIRS})
  link_directories(${${name}_LIBRARY_DIRS})
endfunction(addLib)

foreach(lib ${LIBS})
  addLib(${lib})
endforeach(lib)

target_include_directories(
    nltools INTERFACE
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/..>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_BINARY_DIR}/..>)

include_directories(nltools ${CMAKE_CURRENT_SOURCE_DIR}/../)

set(headers_install_dir ${CMAKE_INSTALL_INCLUDEDIR}/nltools)

install(FILES ${HEADER_FILES} DESTINATION ${headers_install_dir})
install(TARGETS nltools DESTINATION ${CMAKE_INSTALL_LIBDIR})

