cmake_minimum_required(VERSION 3.0)
project(libdevice-config)
set(CMAKE_CXX_STANDARD 14)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

include(FindPkgConfig)
include(GNUInstallDirs)

pkg_search_module(ANDROIDPROPS libandroid-properties)
if(NOT ANDROIDPROPS_FOUND)
    message(WARNING "Did not find android properties, bulding without!")
endif()

set(TARGET deviceinfo)

include_directories(headers)

add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(tools)

install(DIRECTORY configs/ DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/device-info)
