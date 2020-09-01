cmake_minimum_required(VERSION 3.0)
project(deviceinfo)
set(CMAKE_CXX_STANDARD 14)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

include(FindPkgConfig)
include(GNUInstallDirs)

pkg_search_module(ANDROIDPROPS libandroid-properties)

set(TARGET deviceinfo)

include_directories(headers)

set(CONFIG_PATH ${CMAKE_INSTALL_FULL_SYSCONFDIR}/deviceinfo)
install(DIRECTORY configs/ DESTINATION ${CONFIG_PATH})

add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(tools)
