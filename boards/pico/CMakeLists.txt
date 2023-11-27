# Copyright 2023 Girish Palya
# SPDX-License-Identifier: MIT

# cmake_minimum_required(VERSION 3.25 FATAL_ERROR)

# pull in SDK (must be before project)
# option(PICO_SDK_FETCH_FROM_GIT "" on)
# include(cmake/pico_sdk_import.cmake)

# project(pico)

# initialize the sdk
pico_sdk_init()

if (NOT TARGET tinyusb_device)
    message(FATAL_ERROR "TinyUSB is missing")
endif()

# Fetch a shallow clone of qmk repository with only essential files
configure_file(../../cmake/CMakeLists.txt.in qmk_download/CMakeLists.txt @ONLY)
set(QMK_DIRS /quantum/ /tmk_core/)
string(REPLACE ";" "$<SEMICOLON>" ESCAPED_QMK_DIRS "${QMK_DIRS}")
execute_process(COMMAND "${CMAKE_COMMAND}" -G "${CMAKE_GENERATOR}" -D QMK_DIRS=${ESCAPED_QMK_DIRS} .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qmk_download"
)
execute_process(COMMAND "${CMAKE_COMMAND}" --build .
    WORKING_DIRECTORY "${CMAKE_CURRENT_BINARY_DIR}/qmk_download"
)
set(qmk_root ${CMAKE_BINARY_DIR}/qmk_firmware/src)

# qmk

add_library(qmk_core STATIC
    ${qmk_root}/quantum/quantum.c
)
set(qmk_compile_flags -Os -Wall -Werror)
target_compile_options(qmk_core PRIVATE ${qmk_compile_flags})


# pico kb

add_executable(pico_kb
    src/main.c
)

target_compile_features(pico_kb INTERFACE c_std_11)
target_compile_features(pico_kb INTERFACE cxx_std_14)

target_link_libraries(pico_kb
    pico_stdlib
    qmk_core
    # pico_multicore
    # hardware_i2c
)

# Enable usb output, disable uart output
# to open serial port: $ minicom -b 115200 -o -D /dev/ttyACM0
pico_enable_stdio_usb(pico_kb 1)
pico_enable_stdio_uart(pico_kb 0)

# Create uf2, map, hex, bin files
pico_add_extra_outputs(pico_kb)



# # Create library from core files
# add_library(fmk_core INTERFACE)
# target_sources(fmk_core INTERFACE
#     src/main.c
#     )
# target_include_directories(fmk_core INTERFACE
#     ${CMAKE_CURRENT_LIST_DIR}
#     )
# target_link_libraries(fmk_core INTERFACE
#     pico_stdlib
#     pico_multicore
#     hardware_i2c
#     )

# target_compile_definitions(fmk_core INTERFACE
#     DEFAULT_I2C_SDA_PIN=16
#     DEFAULT_I2C_SCL_PIN=17
#     DEFAULT_I2C=i2c0
# )


# target_compile_definitions(on_core_one PRIVATE
#         mainRUN_FREE_RTOS_ON_CORE=1
#         PICO_STACK_SIZE=0x1000
# )

# target_compile_definitions(main_full PRIVATE
#         PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
# )

# add_subdirectory(${free-rtos_SOURCE_DIR} ${free-rtos_BINARY_DIR})
# message(${CMAKE_SOURCE_DIR})

# add_executable(fmk_keyboard main.c ${KEYBOARD_LAYOUT_SRC})

# Add header search paths
# target_include_directories(blink PRIVATE ${CMAKE_CURRENT_LIST_DIR})
#
# # Pull in common dependencies
# target_link_libraries(blink pico_stdlib FreeRTOS-Kernel FreeRTOS-Kernel-Heap4)
#
# # Enable/disable STDIO via USB and UART for debugging
# pico_enable_stdio_usb(blink 1)
# # pico_enable_stdio_uart(blink 1)
#
# # Create map, bin, hex, uf2 file etc.
# pico_add_extra_outputs(blink)
