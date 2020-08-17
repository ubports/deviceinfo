cmake_minimum_required(VERSION 3.0)
project(libdevice-config)
set(CMAKE_CXX_STANDARD 14)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

include(FindPkgConfig)
include(GNUInstallDirs)

option(DISABLE_TESTS "Disable tests" off)

pkg_search_module(ANDROIDPROPS libandroid-properties)

set(TARGET deviceinfo)

include_directories(headers)

add_subdirectory(headers)
add_subdirectory(src)
add_subdirectory(tools)

if (DISABLE_TESTS)
    message(STATUS "Tests disabled")
else()
    include(CTest)
    enable_testing()
    add_subdirectory(tests)
endif()

install(DIRECTORY configs/ DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/device-info)
