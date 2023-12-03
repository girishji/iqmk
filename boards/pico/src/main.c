/*
 * Copyright (c) 2023 Girish Palya
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "pico/stdlib.h"

// #include "keyboard.h"

void protocol_setup(void);
void protocol_pre_init(void);
void protocol_post_init(void);
void protocol_pre_task(void);
void protocol_post_task(void);
void keyboard_init(void);
void keyboard_setup(void);
void keyboard_task(void);
void housekeeping_task(void);

void protocol_init(void) __attribute__((weak));
void protocol_init(void) {
    // protocol_pre_init();

    keyboard_init();

    // protocol_post_init();
}

void protocol_task(void) __attribute__((weak));
void protocol_task(void) {
    // protocol_pre_task();

    keyboard_task();

    // protocol_post_task();
}

int main(void) __attribute__((weak));
int main(void) {
    // platform_setup();
    // protocol_setup();
    keyboard_setup();

    // protocol_init();

    /* Main loop */
    while (true) {
        // protocol_task();

#ifdef QUANTUM_PAINTER_ENABLE
        // Run Quantum Painter animations
        void qp_internal_animation_tick(void);
        qp_internal_animation_tick();
#endif

#ifdef DEFERRED_EXEC_ENABLE
        void deferred_exec_task(void);
        deferred_exec_task();
#endif // DEFERRED_EXEC_ENABLE

        housekeeping_task();
    }
}

// int main() {
//     stdio_init_all();
//     while (true) {
//         printf("Hello, world!\n");
//         sleep_ms(1000);
//     }
// }
