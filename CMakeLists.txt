# Copyright 2023 FMK (Girish Palya)
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

project(
    kefir
    VERSION 0.1.0
    DESCRIPTION "QMK plus"
    HOMEPAGE_URL https://github.com/girishji/kefir
    LANGUAGES ASM C CXX
)

# if(NOT CMAKE_BUILD_TYPE AND NOT CMAKE_CONFIGURATION_TYPES)
#     message("Setting build type to 'RelWithDebInfo' as none was specified.")
#     set(CMAKE_BUILD_TYPE RelWithDebInfo CACHE STRING "Choose the type of build." FORCE)
#     set_property(CACHE CMAKE_BUILD_TYPE PROPERTY STRINGS "Debug" "Release"
#         "MinSizeRel" "RelWithDebInfo")
# endif()

set(CMAKE_BUILD_TYPE Debug CACHE STRING "Choose the type of build." FORCE)

# Fetch a shallow clone of qmk with only essential files
configure_file(cmake/CMakeLists.txt.in qmk-download/CMakeLists.txt @ONLY)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qmk-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qmk-download"
)
set(qmk_root ${CMAKE_BINARY_DIR}/qmk_firmware/src)

# Fetch googletest
include(FetchContent)
# message(STATUS "Fetching GoogleTest repo")
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)


## common compile flags
add_library(compiler_flags INTERFACE)
target_compile_features(compiler_flags INTERFACE c_std_11)
target_compile_features(compiler_flags INTERFACE cxx_std_14)

string(APPEND c_opts "-fno-strict-aliasing;-fdiagnostics-color;"
    "-Wall;-Wstrict-prototypes;-Werror;-fcommon")
set(cxx_opts "-fno-exceptions;-w;-Wall;-Wundef;-Werror")
set(c_clang_opts "-fno-inline-small-functions;-funsigned-bitfields")

target_compile_options(compiler_flags INTERFACE
    -funsigned-char -ffunction-sections -fdata-sections -fshort-enums
    "$<$<COMPILE_LANG_AND_ID:C,Clang>:$<BUILD_INTERFACE:${c_clang_opts}>>"
    "$<$<COMPILE_LANG_AND_ID:CXX,Clang>:$<BUILD_INTERFACE:-funsigned-bitfields>>"
    "$<$<COMPILE_LANGUAGE:C>:$<BUILD_INTERFACE:${c_opts}>>"
    "$<$<COMPILE_LANGUAGE:CXX>:$<BUILD_INTERFACE:${cxx_opts}>>"
)

target_compile_definitions(compiler_flags INTERFACE
    PRINTF_SUPPORT_DECIMAL_SPECIFIERS=0
    PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0
    PRINTF_SUPPORT_LONG_LONG=0
    PRINTF_SUPPORT_WRITEBACK_SPECIFIER=0
    SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS=0
    PRINTF_ALIAS_STANDARD_FUNCTION_NAMES=1
    EEPROM_ENABLE
    EEPROM_VENDOR
    EEPROM_TEST_HARNESS
    MAGIC_KEYCODE_ENABLE
    SEND_STRING_ENABLE
    GRAVE_ESC_ENABLE
    SPACE_CADET_ENABLE
    NO_PRINT
    NO_DEBUG
    PRINTF_SUPPORT_DECIMAL_SPECIFIERS=0
    PRINTF_SUPPORT_EXPONENTIAL_SPECIFIERS=0
    PRINTF_SUPPORT_LONG_LONG=0
    PRINTF_SUPPORT_WRITEBACK_SPECIFIER=0
    SUPPORT_MSVC_STYLE_INTEGER_SPECIFIERS=0
    PRINTF_ALIAS_STANDARD_FUNCTION_NAMES=1
    KEYMAP_C="${qmk_root}/tests/test_common/keymap.c"
)

target_include_directories(compiler_flags INTERFACE
    ${qmk_root}/tmk_core
    ${qmk_root}/quantum
    ${qmk_root}/quantum/keymap_extras
    ${qmk_root}/quantum/process_keycode
    ${qmk_root}/quantum/sequencer
    ${qmk_root}/drivers
    ${qmk_root}/platforms/test/drivers/eeprom
    ${qmk_root}/drivers/eeprom
    ${qmk_root}/quantum/bootmagic
    ${qmk_root}/quantum/send_string
    ${qmk_root}/tests/basic
    ${qmk_root}/quantum/logging
    ${qmk_root}/lib/printf/src
    ${qmk_root}/lib/printf/src/printf
    ${qmk_root}/platforms
    ${qmk_root}/platforms/test
    ${qmk_root}/platforms/test/drivers
    ${qmk_root}/tmk_core/protocol
    ${qmk_root}/lib/printf/src
    ${qmk_root}/lib/printf/src/printf
    ${qmk_root}/tests/test_common
)

set(qmk_common_srcs
    ${qmk_root}/quantum/quantum.c
    ${qmk_root}/quantum/bitwise.c
    ${qmk_root}/quantum/led.c
    ${qmk_root}/quantum/action.c
    ${qmk_root}/quantum/action_layer.c
    ${qmk_root}/quantum/action_tapping.c
    ${qmk_root}/quantum/action_util.c
    ${qmk_root}/quantum/eeconfig.c
    ${qmk_root}/quantum/keyboard.c
    ${qmk_root}/quantum/keymap_common.c
    ${qmk_root}/quantum/keycode_config.c
    ${qmk_root}/quantum/sync_timer.c
    ${qmk_root}/quantum/logging/debug.c
    ${qmk_root}/quantum/logging/sendchar.c
    ${qmk_root}/quantum/logging/print.c
    ${qmk_root}/quantum/bootmagic/magic.c
    ${qmk_root}/quantum/debounce/sym_defer_g.c
    ${qmk_root}/lib/printf/src/printf/printf.c
    ${qmk_root}/platforms/test/eeprom.c
    ${qmk_root}/quantum/process_keycode/process_magic.c
    ${qmk_root}/quantum/send_string/send_string.c
    ${qmk_root}/quantum/process_keycode/process_grave_esc.c
    ${qmk_root}/quantum/process_keycode/process_space_cadet.c
    ${qmk_root}/platforms/suspend.c
    ${qmk_root}/platforms/synchronization_util.c
    ${qmk_root}/platforms/timer.c
    ${qmk_root}/platforms/test/hardware_id.c
    ${qmk_root}/platforms/test/platform.c
    ${qmk_root}/platforms/test/suspend.c
    ${qmk_root}/platforms/test/timer.c
    ${qmk_root}/platforms/test/bootloaders/none.c
    ${qmk_root}/tmk_core/protocol/host.c
    ${qmk_root}/tmk_core/protocol/report.c
    ${qmk_root}/tmk_core/protocol/usb_device_state.c
    ${qmk_root}/tmk_core/protocol/usb_util.c
    ${qmk_root}/quantum/keymap_introspection.c
)

set(tests_common_srcs
    ${qmk_root}/tests/test_common/matrix.c
    ${qmk_root}/tests/test_common/test_driver.cpp
    ${qmk_root}/tests/test_common/keyboard_report_util.cpp
    ${qmk_root}/tests/test_common/keycode_util.cpp
    ${qmk_root}/tests/test_common/keycode_table.cpp
    ${qmk_root}/tests/test_common/test_fixture.cpp
    ${qmk_root}/tests/test_common/test_keymap_key.cpp
    ${qmk_root}/tests/test_common/test_logger.cpp
    ${qmk_root}/tests/test_common/main.cpp
)

enable_testing()
include(GoogleTest)

foreach(fixture
        auto_shift
        autocorrect
        basic
        caps_word
        caps_word/caps_word_autoshift
        caps_word/caps_word_combo
        caps_word/caps_word_invert_on_shift
        caps_word/caps_word_unicodemap
        combo
        leader
        leader/leader_no_initial_timeout
        leader/leader_per_key_timeout
        leader/leader_strict_key_processing
        no_tapping/no_action_tapping
        no_tapping/no_mod_tap_mods
        repeat_key
        repeat_key/alt_repeat_key
        repeat_key/repeat_key_combo
        secure
        tap_dance
    )

    # cmake discourages glob, but in this situation it should be alright
    # https://cmake.org/cmake/help/latest/command/file.html#glob
    set(globdir ${qmk_root}/tests/${fixture})
    file(GLOB tests_srcs CONFIGURE_DEPENDS "${globdir}/*.h" "${globdir}/*.cpp")

    cmake_path(GET fixture FILENAME barename)
    set(tname "test_${barename}")
    add_executable(${tname}
        ${qmk_common_srcs}
        ${tests_common_srcs}
        ${tests_srcs}
    )
    target_link_libraries(${tname}
        compiler_flags
        GTest::gtest
        GTest::gmock
    )
    gtest_discover_tests(${tname})
endforeach()

## basic
target_compile_options(test_basic PRIVATE -include ${qmk_root}/tests/basic/config.h)

## auto_shift
target_sources(test_auto_shift PRIVATE ${qmk_root}/quantum/process_keycode/process_auto_shift.c)
target_compile_definitions(test_auto_shift PRIVATE AUTO_SHIFT_ENABLE)
target_compile_options(test_auto_shift PRIVATE -include ${qmk_root}/tests/auto_shift/config.h)

## autocorrect
target_sources(test_autocorrect PRIVATE
    ${qmk_root}/quantum/process_keycode/process_autocorrect.c)
target_compile_options(test_autocorrect PRIVATE -include ${qmk_root}/tests/autocorrect/config.h)
target_compile_definitions(test_autocorrect PRIVATE AUTOCORRECT_ENABLE)

## caps_word
set(caps_word_srcs
    ${qmk_root}/quantum/caps_word.c
    ${qmk_root}/quantum/process_keycode/process_caps_word.c
)
target_sources(test_caps_word PRIVATE ${caps_word_srcs})
target_compile_options(test_caps_word PRIVATE -include ${qmk_root}/tests/caps_word/config.h)
target_compile_definitions(test_caps_word PRIVATE CAPS_WORD_ENABLE SPACE_CADET_ENABLE)

## caps_word_autoshift
target_sources(test_caps_word_autoshift PRIVATE
    ${caps_word_srcs}
    ${qmk_root}/quantum/process_keycode/process_auto_shift.c
)
target_compile_options(test_caps_word_autoshift PRIVATE -include ${qmk_root}/tests/caps_word/caps_word_autoshift/config.h)
target_compile_definitions(test_caps_word_autoshift PRIVATE CAPS_WORD_ENABLE AUTO_SHIFT_ENABLE)

## caps_word_combo
target_sources(test_caps_word_combo PRIVATE
    ${caps_word_srcs}
    ${qmk_root}/quantum/process_keycode/process_combo.c
    ${qmk_root}/quantum/process_keycode/process_auto_shift.c
)
target_compile_options(test_caps_word_combo PRIVATE -include ${qmk_root}/tests/caps_word/caps_word_combo/config.h)
target_compile_definitions(test_caps_word_combo PRIVATE
    CAPS_WORD_ENABLE AUTO_SHIFT_ENABLE COMBO_ENABLE
    INTROSPECTION_KEYMAP_C="${qmk_root}/tests/caps_word/caps_word_combo/test_combos.c"
)

## caps_word_invert_on_shift
target_sources(test_caps_word_invert_on_shift PRIVATE ${caps_word_srcs})
target_compile_options(test_caps_word_invert_on_shift PRIVATE -include ${qmk_root}/tests/caps_word/caps_word_invert_on_shift/config.h)
target_compile_definitions(test_caps_word_invert_on_shift PRIVATE CAPS_WORD_ENABLE)

## caps_word_unicodemap
target_sources(test_caps_word_unicodemap PRIVATE
    ${caps_word_srcs}
    ${qmk_root}/quantum/unicode/unicodemap.c
    ${qmk_root}/quantum/process_keycode/process_unicodemap.c
    ${qmk_root}/quantum/process_keycode/process_unicode_common.c
    ${qmk_root}/quantum/unicode/unicode.c
    ${qmk_root}/quantum/unicode/utf8.c
)
target_compile_options(test_caps_word_unicodemap PRIVATE -include ${qmk_root}/tests/caps_word/caps_word_unicodemap/config.h)
target_compile_definitions(test_caps_word_unicodemap PRIVATE CAPS_WORD_ENABLE UNICODEMAP_ENABLE UNICODE_COMMON_ENABLE)
target_include_directories(test_caps_word_unicodemap PRIVATE ${qmk_root}/quantum/unicode)

## combo
target_sources(test_combo PRIVATE ${qmk_root}/quantum/process_keycode/process_combo.c)
target_compile_options(test_combo PRIVATE -include ${qmk_root}/tests/combo/config.h)
target_compile_definitions(test_combo PRIVATE COMBO_ENABLE
    INTROSPECTION_KEYMAP_C="${qmk_root}/tests/combo/test_combos.c"
)

## leader and leader/*
foreach(leader_fixture
        leader
        leader/leader_no_initial_timeout
        leader/leader_per_key_timeout
        leader/leader_strict_key_processing
)
    cmake_path(GET leader_fixture FILENAME barename)
    set(tname "test_${barename}")
    target_sources(${tname} PRIVATE
        ${qmk_root}/quantum/leader.c
        ${qmk_root}/quantum/process_keycode/process_leader.c
        ${qmk_root}/tests/leader/leader_sequences.c
    )
    target_compile_options(${tname} PRIVATE -include ${qmk_root}/tests/${leader_fixture}/config.h)
    target_compile_definitions(${tname} PRIVATE LEADER_ENABLE)
endforeach()

## no_action_tapping
target_compile_options(test_no_action_tapping PRIVATE
    -include ${qmk_root}/tests/no_tapping/no_action_tapping/config.h
)

## no_mod_tap_mods
target_compile_options(test_no_mod_tap_mods PRIVATE
    -include ${qmk_root}/tests/no_tapping/no_mod_tap_mods/config.h
)

## repeat_key
set(repeat_key_srcs
    ${qmk_root}/quantum/process_keycode/process_repeat_key.c
    ${qmk_root}/quantum/repeat_key.c
)
target_sources(test_repeat_key PRIVATE
    ${repeat_key_srcs}
    ${qmk_root}/quantum/process_keycode/process_auto_shift.c
)
target_compile_options(test_repeat_key PRIVATE -include ${qmk_root}/tests/repeat_key/config.h)
target_compile_definitions(test_repeat_key PRIVATE REPEAT_KEY_ENABLE AUTO_SHIFT_ENABLE)

## alt_repeat_key
target_sources(test_alt_repeat_key PRIVATE ${repeat_key_srcs})
target_compile_options(test_alt_repeat_key PRIVATE -include ${qmk_root}/tests/repeat_key/alt_repeat_key/config.h)
target_compile_definitions(test_alt_repeat_key PRIVATE REPEAT_KEY_ENABLE EXTRAKEY_ENABLE)

## repeat_key_combo
target_sources(test_repeat_key_combo PRIVATE
    ${repeat_key_srcs}
    ${qmk_root}/quantum/process_keycode/process_combo.c
)
target_compile_options(test_repeat_key_combo PRIVATE -include ${qmk_root}/tests/repeat_key/repeat_key_combo/config.h)
target_compile_definitions(test_repeat_key_combo PRIVATE REPEAT_KEY_ENABLE COMBO_ENABLE
    INTROSPECTION_KEYMAP_C="${qmk_root}/tests/repeat_key/repeat_key_combo/test_combos.c"
)

## secure
target_sources(test_secure PRIVATE
    ${qmk_root}/quantum/process_keycode/process_secure.c
    ${qmk_root}/quantum/secure.c
)
target_compile_options(test_secure PRIVATE -include ${qmk_root}/tests/secure/config.h)
target_compile_definitions(test_secure PRIVATE SECURE_ENABLE)

## tap_dance
target_sources(test_tap_dance PRIVATE
    ${qmk_root}/tests/tap_dance/examples.c
    ${qmk_root}/quantum/process_keycode/process_tap_dance.c
)
target_compile_options(test_tap_dance PRIVATE -include ${qmk_root}/tests/tap_dance/config.h)
target_compile_definitions(test_tap_dance PRIVATE TAP_DANCE_ENABLE)
