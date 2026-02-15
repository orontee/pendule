cmake_minimum_required(VERSION 3.25)
cmake_policy(SET CMP0167 NEW)

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
pkg_check_modules(sigc++-3.0 REQUIRED IMPORTED_TARGET sigc++-3.0)
find_package(Boost 1.81 REQUIRED
  COMPONENTS coroutine json
)

add_executable(pendule
  src/clock.cc
  src/main.cc
)

target_include_directories(pendule
  PRIVATE
  src/
  ${Boost_INCLUDE_DIRS}
  ${gtkmm-4.0_INCLUDE_DIRECTORIES}
  ${sigc++-3.0_INCLUDE_DIRECTORIES}
)

target_compile_options(pendule
  PRIVATE
  ${gtkmm-4.0_CFLAGS}
  ${sigc++-3.0_CFLAGS}
)

target_link_libraries(pendule
  PRIVATE
  Boost::coroutine
  Boost::json
  ${gtkmm-4.0_LDFLAGS}
  ${sigc++-3.0_LDFLAGS}
)

install(TARGETS pendule DESTINATION ${CMAKE_INSTALL_BINDIR})

feature_summary(WHAT ALL)
