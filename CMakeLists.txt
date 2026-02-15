cmake_minimum_required(VERSION 3.14)
project(pendule VERSION 1.0.0 LANGUAGES CXX)
set(PROJEC_HOMEPAGE_URL "https://github.com/orontee/pendule")

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include(FeatureSummary)
include(FindPkgConfig)
if(NOT PKG_CONFIG_FOUND)
    message(FATAL_ERROR "pkg-config not found!" )
endif()
include(GNUInstallDirs)

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

install(TARGETS pendule
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
  PROGRAMS
  bin/hdmi-monitor.sh
  bin/hdmi-power.sh
  bin/start_pendule.sh
  DESTINATION ${CMAKE_INSTALL_BINDIR}
)
install(
  FILES
  systemd/system/hdmi-monitor.service
  systemd/system/hdmi-power.service
  systemd/system/pendule.service
  systemd/system/start-hdmi-power.service
  systemd/system/start-hdmi-power.timer
  systemd/system/stop-hdmi-power.service
  systemd/system/stop-hdmi-power.timer
  DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/systemd/system
)

set(CPACK_PACKAGE_NAME ${PROJECT_NAME})
set(CPACK_PACKAGE_VERSION ${PROJECT_VERSION})
set(CPACK_PACKAGE_CONTACT "Matthias Meulien <orontee@gmail.com>")
set(CPACK_PACKAGE_DESCRIPTION_SUMMARY "Simple analog clock")

set(CPACK_DEBIAN_PACKAGE_DEPENDS
  "libgtkmm-4.0-0"
)

feature_summary(WHAT ALL)
set(CPACK_GENERATOR "DEB")
set(CPACK_DEBIAN_PACKAGE_CONTROL_EXTRA "${CMAKE_CURRENT_SOURCE_DIR}/debian/postinst")
set(CPACK_PACKAGING_INSTALL_PREFIX "/")
include(CPack)
