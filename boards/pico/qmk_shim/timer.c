#include "timer.h"

static uint32_t current_time = 0;

void timer_init(void) {
    current_time = 0;
}

void timer_clear(void) {
    current_time = 0;
}

uint16_t timer_read(void) {
    return current_time & 0xFFFF;
}
uint32_t timer_read32(void) {
    return current_time;
}
uint16_t timer_elapsed(uint16_t last) {
    return TIMER_DIFF_16(timer_read(), last);
}
uint32_t timer_elapsed32(uint32_t last) {
    return TIMER_DIFF_32(timer_read32(), last);
}

void set_time(uint32_t t) {
    current_time = t;
}
void advance_time(uint32_t ms) {
    current_time += ms;
}

void wait_ms(uint32_t ms) {
    advance_time(ms);
}
