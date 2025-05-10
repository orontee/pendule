cmake_minimum_required(VERSION 3.14)
project(wlclock)

set(CMAKE_CXX_STANDARD 17)

include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config not found!" )
endif()

pkg_check_modules(gtkmm-4.0 REQUIRED IMPORTED_TARGET gtkmm-4.0)

add_executable(pendule
        clock.cc
        clock.h
        main.cc)

target_link_libraries(pendule PkgConfig::gtkmm-4.0)

install(TARGETS pendule DESTINATION ${CMAKE_INSTALL_BINDIR})
