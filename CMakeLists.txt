# Copyright 2023 Girish Palya
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
    kefir
    VERSION 0.1.0
    DESCRIPTION "QMK plus"
    HOMEPAGE_URL https://github.com/girishji/kefir
    LANGUAGES ASM C CXX
)

if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
    message("Setting build type to 'RelWithDebInfo' as none was specified.")
    set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
        "MinSizeRel" "RelWithDebInfo")
endif()

# set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

if(NOT DEFINED QMK_BUILD_TYPE)
    message("Setting build type to 'Test' as none was specified.")
    set(QMK_BUILD_TYPE Test CACHE STRING "Choose the type of build." FORCE)
    set_property(CACHE QMK_BUILD_TYPE PROPERTY STRINGS "Test" "Pico")
endif()

set (CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

if(QMK_BUILD_TYPE STREQUAL Test)
    include(qmk_test)
endif()
