#include "wait.h"
#include "pico/stdlib.h"

void wait_ms(uint32_t ms) {
    sleep_ms(ms);
}

// void wait_us(uint64_t us) {
void wait_us(uint32_t us) {
    // busy_wait_us(us);
    busy_wait_us_32(us);
}
