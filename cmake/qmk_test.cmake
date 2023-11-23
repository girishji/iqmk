# Copyright 2023 Girish Palya
# SPDX-License-Identifier: MIT

# Fetch a shallow clone of qmk with only essential files
configure_file(cmake/CMakeLists.txt.in qmk_download/CMakeLists.txt @ONLY)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qmk_download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qmk_download"
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

include(qmk_common)

set(test_common_srcs
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
    file(GLOB test_srcs CONFIGURE_DEPENDS "${globdir}/*.h" "${globdir}/*.cpp")

    cmake_path(GET fixture FILENAME barename)
    set(tname "test_${barename}")
    add_executable(${tname}
        ${qmk_common_srcs}
        ${test_common_srcs}
        ${test_srcs}
    )
    target_link_libraries(${tname}
        qmk_compiler_flags
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

