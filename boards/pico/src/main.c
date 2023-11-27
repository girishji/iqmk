/*
 * Copyright (c) 2023 Girish Palya
 *
 * SPDX-License-Identifier: MIT
 */

#include <stdio.h>
#include "pico/stdlib.h"

int main() {
    stdio_init_all();
    while (true) {
        printf("Hello, world!\n");
        sleep_ms(1000);
    }
}
