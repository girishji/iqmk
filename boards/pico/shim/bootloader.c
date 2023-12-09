#include "bootloader.h"
#include "pico/stdlib.h"

#define AIRCR_Register (*((volatile uint32_t*)(PPB_BASE + 0x0ED0C)))

__attribute__((weak)) void mcu_reset(void) {
    AIRCR_Register = 0x5FA0004;
}

__attribute__((weak)) void bootloader_jump(void) {}
