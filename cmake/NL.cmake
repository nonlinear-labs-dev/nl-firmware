
function(find_systemd)
    find_package(PkgConfig REQUIRED)
    pkg_check_modules(SYSTEMD "systemd")

    if (SYSTEMD_FOUND AND !DEV_PC)
        execute_process(COMMAND ${PKG_CONFIG_EXECUTABLE} --variable=systemdsystemunitdir systemd OUTPUT_VARIABLE _SYSTEMD_SERVICES_INSTALL_DIR)
        string(REGEX REPLACE "[ \t\n]+" "" SYSTEMD_SERVICES_INSTALL_DIR "${_SYSTEMD_SERVICES_INSTALL_DIR}")
    else ()
        set(_SYSTEMD_SERVICES_INSTALL_DIR ${CMAKE_INSTALL_PREFIX}/lib/systemd/system)
    endif ()

    set(SYSTEMD_SERVICES_INSTALL_DIR ${_SYSTEMD_SERVICES_INSTALL_DIR} PARENT_SCOPE)
endfunction()

function(enable_ccache)
    find_program(CCACHE_PROGRAM ccache)

    if (CCACHE_PROGRAM)
        set_property(GLOBAL PROPERTY RULE_LAUNCH_COMPILE "${CCACHE_PROGRAM}")
    endif ()
endfunction()


function(prepare_nltools_1_and_2)
    set(CMAKE_INSTALL_RPATH
            "${C15_INSTALL_PATH}/nltools"
            "${C15_INSTALL_PATH}/nltools-2"
            PARENT_SCOPE)
endfunction()

function(prepare_nltools)
    set(CMAKE_INSTALL_RPATH
            ${C15_INSTALL_PATH}/nltools
            PARENT_SCOPE)
endfunction()

function(prepare_nltools_2)
    set(CMAKE_INSTALL_RPATH
            ${C15_INSTALL_PATH}/nltools-2
            PARENT_SCOPE)
endfunction()

function(add_nltools TARGET)
    target_link_libraries(${TARGET} PUBLIC nltools)
    target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/projects/shared/nltools/include/)
endfunction()

function(add_nltools_2 TARGET)
    target_link_libraries(${TARGET} PUBLIC nltools2)
    target_include_directories(${TARGET} PUBLIC ${CMAKE_SOURCE_DIR}/projects/shared/nltools-2/include/)
endfunction()

function(add_nltools_1_and_2 TARGET)
    add_nltools(${TARGET})
    add_nltools_2(${TARGET})
endfunction()