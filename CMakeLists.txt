# Copyright 2023 FMK (Girish Palya)
# SPDX-License-Identifier: MIT

cmake_minimum_required(VERSION 3.18 FATAL_ERROR)

# add_compile_options(-g -Og -w -Wall -Wundef -Werror)

project(
    kefir
    VERSION 0.1.0
    DESCRIPTION "Essential QMK Sans Middleware"
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

# Specify include dir
# ExternalProject_Get_Property(qmk SOURCE_DIR)
# get_filename_component(qmk_root ${SOURCE_DIR} DIRECTORY)
# message("giris  qmk root ${qmk_root}")
# set(GLM_INCLUDE_DIRS ${source_dir}/../ PARENT_SCOPE)

# Fetch a shallow clone of qmk with only essential files
configure_file(cmake/CMakeLists.txt.in qmk-download/CMakeLists.txt @ONLY)
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qmk-download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_BINARY_DIR}/qmk-download"
)


# Fetch googletest
include(FetchContent)
# message(STATUS "Fetching GoogleTest repo")
FetchContent_Declare(googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
# For Windows: Prevent overriding the parent project's compiler/linker settings
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

set(qmk_root ${CMAKE_BINARY_DIR}/qmk_firmware/src)

add_library(qmkcore
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
    ${qmk_root}/tests/test_common/matrix.c
    )
# XXX matrix.c above is in tests/

target_compile_features(qmkcore PRIVATE c_std_11)
target_compile_features(qmkcore PRIVATE cxx_std_14)

set(qmkcore_compile_options
    -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections
    -fshort-enums -fno-inline-small-functions -fno-strict-aliasing
    -fdiagnostics-color -Wall -Wstrict-prototypes -Werror -fcommon
    -include ${qmk_root}/tests/basic/config.h
    )
target_compile_options(qmkcore PRIVATE ${qmkcore_compile_options})

set(common_compile_definitions
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

target_compile_definitions(qmkcore PRIVATE ${common_compile_definitions})

set(common_include_directories
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

target_include_directories(qmkcore PRIVATE ${common_include_directories})

# tests/test_common/common_config.h 

set(tests_test_common_srcs
    ${qmk_root}/tests/test_common/test_driver.cpp
    ${qmk_root}/tests/test_common/keyboard_report_util.cpp
    ${qmk_root}/tests/test_common/keycode_util.cpp
    ${qmk_root}/tests/test_common/keycode_table.cpp
    ${qmk_root}/tests/test_common/test_fixture.cpp
    ${qmk_root}/tests/test_common/test_keymap_key.cpp
    ${qmk_root}/tests/test_common/test_logger.cpp
    )

set(tests_basic_srcs
    ${qmk_root}/tests/basic/test_action_layer.cpp
    ${qmk_root}/tests/basic/test_keycode_util.cpp
    ${qmk_root}/tests/basic/test_keypress.cpp
    ${qmk_root}/tests/basic/test_one_shot_keys.cpp
    ${qmk_root}/tests/basic/test_tapping.cpp
    )

add_executable(testqmk
    ${tests_test_common_srcs}
    ${tests_basic_srcs}
    ${qmk_root}/tests/test_common/main.cpp
    )

set(tests_compile_options
    -funsigned-char -funsigned-bitfields -ffunction-sections -fdata-sections
    -fshort-enums -fno-exceptions -w -Wall -Wundef -Werror
    -include ${qmk_root}/tests/basic/config.h
    )
# target_compile_options(qmktest PRIVATE ${qmktest_compile_options})

# target_compile_features(qmktest PRIVATE c_std_11)
# target_compile_features(qmktest PRIVATE cxx_std_14)
# target_compile_definitions(qmktest PRIVATE ${common_definitions})
# target_include_directories(qmktest PRIVATE ${common_include_directories})

# target_link_libraries(qmktest PRIVATE GTest::gmock)


# target_link_directories   (qmktest PRIVATE ${slate_dir}/lib)
# target_link_libraries     (qmktest PRIVATE slate blaspp lapackpp)
# set_property(TARGET 1D_DFT PROPERTY BUILD_RPATH "${slate_dir}/lib")

# string(REPLACE ";" "$<SEMICOLON>" escaped_dirs "${dirs_list}")

# tests/test_common/common_config.h 
# . removed

target_compile_options(testqmk PRIVATE ${tests_compile_options})
target_compile_definitions(testqmk PRIVATE ${common_compile_definitions})
target_include_directories(testqmk PRIVATE ${common_include_directories})
target_compile_features(testqmk PRIVATE c_std_11)
target_compile_features(testqmk PRIVATE cxx_std_14)

target_link_libraries(testqmk
  qmkcore
  GTest::gtest
  GTest::gmock
)

enable_testing()
include(GoogleTest)
gtest_discover_tests(testqmk)
