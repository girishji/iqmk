# Copyright 2023 Girish Palya
# SPDX-License-Identifier: MIT

## common compile flags
add_library(qmk_compiler_flags INTERFACE)
target_compile_features(qmk_compiler_flags INTERFACE c_std_11)
target_compile_features(qmk_compiler_flags INTERFACE cxx_std_14)

string(APPEND c_opts "-fno-strict-aliasing;-fdiagnostics-color;"
    "-Wall;-Wstrict-prototypes;-Werror;-fcommon")
set(cxx_opts "-fno-exceptions;-w;-Wall;-Wundef;-Werror")
set(c_clang_opts "-fno-inline-small-functions;-funsigned-bitfields")

target_compile_options(qmk_compiler_flags INTERFACE
    -funsigned-char -ffunction-sections -fdata-sections -fshort-enums
    "$<$<COMPILE_LANG_AND_ID:C,Clang>:$<BUILD_INTERFACE:${c_clang_opts}>>"
    "$<$<COMPILE_LANG_AND_ID:CXX,Clang>:$<BUILD_INTERFACE:-funsigned-bitfields>>"
    "$<$<COMPILE_LANGUAGE:C>:$<BUILD_INTERFACE:${c_opts}>>"
    "$<$<COMPILE_LANGUAGE:CXX>:$<BUILD_INTERFACE:${cxx_opts}>>"
)

target_compile_definitions(qmk_compiler_flags INTERFACE
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

target_include_directories(qmk_compiler_flags INTERFACE
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
