#include "timer.h"
#include "pico/stdlib.h"

uint16_t timer_read(void) {
    return (uint16_t)timer_read32();
}

// Time stamp in milliseconds
uint32_t timer_read32(void) {
    return to_ms_since_boot(get_absolute_time());
}

uint16_t timer_elapsed(uint16_t last) {
    return TIMER_DIFF_16(timer_read(), last);
}

uint32_t timer_elapsed32(uint32_t last) {
    return TIMER_DIFF_32(timer_read32(), last);
}

void timer_init(void) {
}

void timer_clear(void) {
}
