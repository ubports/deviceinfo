cmake_minimum_required(VERSION 3.0)
project(deviceinfo)
set(CMAKE_CXX_STANDARD 14)

set(VERSION_MAJOR 0)
set(VERSION_MINOR 1)
set(VERSION_PATCH 0)

include(FindPkgConfig)
include(GNUInstallDirs)

option(DISABLE_TESTS "Disable tests" off)

pkg_search_module(YAMLCPP yaml-cpp REQUIRED)
pkg_search_module(ANDROIDPROPS libandroid-properties)
if(NOT ANDROIDPROPS_FOUND)
    message(WARNING "Did not find android properties, bulding without!")
endif()

set(TARGET deviceinfo)

include_directories(headers)

set(CONFIG_PATH ${CMAKE_INSTALL_FULL_SYSCONFDIR}/deviceinfo CACHE STRING "")
install(DIRECTORY configs/ DESTINATION ${CONFIG_PATH})

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

# Coverage
#####################################################################
# Enable code coverage calculation with gcov/gcovr/lcov
# Usage:
#  * Switch build type to coverage (use ccmake or cmake-gui)
#  * Invoke make, make test, make coverage (or ninja if you use that backend)
#  * Find html report in subdir coveragereport
#  * Find xml report feasible for jenkins in coverage.xml
#####################################################################
find_package(CoverageReport)
