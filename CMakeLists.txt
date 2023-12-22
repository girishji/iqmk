# Copyright 2023 Girish Palya
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

if(NOT DEFINED QMK_BUILD_TYPE)
    # message("Setting build type to 'Test' as none was specified.")
    # set(QMK_BUILD_TYPE Test CACHE STRING "Choose the type of build." FORCE)
    set(QMK_BUILD_TYPE Pico CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE QMK_BUILD_TYPE PROPERTY STRINGS "Test" "Pico")
endif()

if(QMK_BUILD_TYPE STREQUAL Pico)
    # pull in SDK (must be before project())
    option(PICO_SDK_FETCH_FROM_GIT "" on)
    include(cmake/pico_sdk_import.cmake)
endif()

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()

# set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

project(
    iqmk
    VERSION 0.1.0
    DESCRIPTION "QMK plus"
    HOMEPAGE_URL https://github.com/girishji/iqmk
    LANGUAGES ASM C CXX
)

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(QMK_BUILD_TYPE STREQUAL Test)
    include(qmk_tests)
elseif(QMK_BUILD_TYPE STREQUAL Pico)
    add_subdirectory(boards/pico)
endif()
