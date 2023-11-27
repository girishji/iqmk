/*
 * Copyright (c) 2022 FMK (Girish Palya)
 *
 * SPDX-License-Identifier: MIT
 */

/* Standard includes. */
#include <stdio.h>

/* Kernel includes. */
#include "FreeRTOS.h"
#include "semphr.h"

/* SDK includes. */
#include "pico/stdlib.h"

void vPrintString(const char* pcString);

/*------------------------------------------------------------*/

void vAssertCalled(const char* pcFile, uint32_t ulLine)
{
    /* pcFile holds the name of the source file that contains the line that
     * detected the error, and ulLine holds the line number in the source file. */
    char buf[256];
    snprintf(buf, sizeof(buf), "Assert failed: %s: %lu\n", pcFile, ulLine);
    vPrintString(buf);

    /* Disable interrupts so the tick interrupt stops executing, then sit in a
    loop so execution does not move past the line that failed the assertion. */
    taskDISABLE_INTERRUPTS();
    for (;;)
        ;
}

/*------------------------------------------------------------*/

/* If not using Heap_3 allocation pvPortMalloc() nad pvPortFree() should be used
 * for allocation and freeing. Some standard functions may still call the the
 * malloc/free couple. Declare them locally making sure that they have a
 * C-linking (and not a C++ linking). Calling malloc() will now result in link
 * error.
 */
extern void non_existent_function(void);

void* malloc(size_t size)
{
    (void)size;
    non_existent_function();
    return NULL;
}

void free(void* pvMemory)
{
    (void)pvMemory;
    non_existent_function();
}

/*------------------------------------------------------------*/

/* Make printing thread safe, so when multiple threads try to print at the same
 * time output will not get interleaved. */
void vPrintString(const char* pcString)
{
    static SemaphoreHandle_t xMutex = NULL;
    // Block for 100 ms
    static const TickType_t maxDelay = 100 / portTICK_PERIOD_MS;

    if (xMutex == NULL) {
        /* As we are using the semaphore for mutual exclusion we create a mutex
         * semaphore rather than a binary semaphore. */
        xMutex = xSemaphoreCreateMutex();
    }

    // Wait at most maxDelay to acquire mutex
    if (xMutex != NULL) {
        if (xSemaphoreTake(xMutex, maxDelay) == pdTRUE) {
            printf("%s", pcString);
            fflush(stdout);
            xSemaphoreGive(xMutex);
        }
    }
}
