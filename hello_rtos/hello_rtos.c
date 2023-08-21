/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include "pico/stdlib.h"

#include "FreeRTOS.h"
#include "task.h"

#define TEST_TASK_PRIORITY				( tskIDLE_PRIORITY + 1UL )

void main_task(__unused void *params) {

    while(true) {
        vTaskDelay(1000);
	printf("Main is idling...\n");
    }
}

void vLaunch( void) {
    TaskHandle_t task;
    xTaskCreate(main_task, "TestMainThread", configMINIMAL_STACK_SIZE, NULL, TEST_TASK_PRIORITY, &task);

    // we must bind the main task to one core (well at least while the init is called)
    //vTaskCoreAffinitySet(task, 1);

    /* Start the tasks and timer running. */
    vTaskStartScheduler();
}

int main() {
    stdio_init_all();
    vLaunch();
}
