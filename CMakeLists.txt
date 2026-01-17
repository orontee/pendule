cmake_minimum_required(VERSION 3.14)
project(pendule VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FeatureSummary)
include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config not found!" )
endif()

pkg_check_modules(gtkmm-4.0 REQUIRED IMPORTED_TARGET gtkmm-4.0)

add_executable(pendule
  src/clock.cc
  src/main.cc
)

target_include_directories(pendule
  PRIVATE
  src/
)
target_link_libraries(pendule PkgConfig::gtkmm-4.0)

install(TARGETS pendule DESTINATION ${CMAKE_INSTALL_BINDIR})

feature_summary(WHAT ALL)
