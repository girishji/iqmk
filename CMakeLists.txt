# Copyright 2023 FMK (Girish Palya)
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

project(
    kefir
    VERSION 0.1.0
    DESCRIPTION "Essential QMK Sans Middleware"
    HOMEPAGE_URL https://github.com/girishji/libQMK
    LANGUAGES ASM C CXX
)

# Fetch external repos
include(FetchContent)

# Fetch QMK
# message(STATUS "Fetching QMK repo (may take a few minutes)")
# FetchContent_Declare(
#     qmk_firmware
#     GIT_REPOSITORY https://github.com/qmk/qmk_firmware.git
#     GIT_TAG d2892685c52a8a0892b28872cfc43e8f7fa37465 # Oct 26, 2023
# )
# FetchContent_MakeAvailable(qmk_firmware)

# Fetch googletest
message(STATUS "Fetching GoogleTest repo")
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

# Build library
# add_library(libqmk INTERFACE)
# target_compile_features(libqmk PRIVATE c_std_11)
# target_compile_features(libqmk PRIVATE cxx_std_17)

# # include('sources.cmake')
# target_sources(libqmk PRIVATE
#     ${qmk_repo_SOURCE_DIR}/quantum/keyboard.c
# )

# target_compile_definitions(libqmk PRIVATE
#     DEFAULT_I2C_SDA_PIN=16
#     DEFAULT_I2C_SCL_PIN=17
#     DEFAULT_I2C=i2c0
# )

# Tests
enable_testing()

add_executable(
    qmktest
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/quantum.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/bitwise.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/led.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/action.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/action_layer.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/action_tapping.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/action_util.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/eeconfig.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/keyboard.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/keymap_common.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/keycode_config.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/sync_timer.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/logging/debug.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/logging/sendchar.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/logging/print.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/bootmagic/magic.c
    # ${qmk_firmware_SOURCE_DIR}/quantum/quantum/debounce/sym_defer_g.c
    # ${qmk_firmware_SOURCE_DIR}/tests/test_common/main.cpp
    tests/hidkb_test.cc
)
target_link_libraries(
  qmktest
  GTest::gtest_main
  # GTest::gtest
)

include(GoogleTest)
gtest_discover_tests(qmktest)
