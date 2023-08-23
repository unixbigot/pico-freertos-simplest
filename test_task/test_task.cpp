#include <stdio.h>
#include "pico/stdlib.h"

#include <FreeRTOS.h>
#include <task.h>
#include <queue.h>

#include "log.h"
#include "Task.h"

#define GPIO_PIN 20

volatile QueueHandle_t queue = NULL;

// Set a delay time of exactly 500ms
const TickType_t ms_delay = 500 / portTICK_PERIOD_MS;

const char *getName() { return "GLOBAL"; }

/**
 * @brief Show basic device info.
 */
void log_device_info(void) {
    printf("App: %s %s\n Build: %i\n", APP_NAME, APP_VERSION, BUILD_NUMBER);
}


/**
 * @brief Repeatedly flash an LED connected to GPIO pin 20
 *        based on the value passed via the inter-task queue.
 */
void led_task_gpio(void* unused_arg) {
    // This variable will take a copy of the value
    // added to the FreeRTOS xQueue
    uint8_t passed_value_buffer = 0;
    DEBUG("BEGIN led_task_gpio");
    
    // Configure the GPIO LED
    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, GPIO_OUT);
    
    while (true) {
        // Check for an item in the FreeRTOS xQueue
        if (xQueueReceive(queue, &passed_value_buffer, portMAX_DELAY) == pdPASS) {
            // Received a value so flash the GPIO LED accordingly
            // (NOT the sent value)
            if (passed_value_buffer) DEBUG("GPIO LED FLASH");
            gpio_put(GPIO_PIN, passed_value_buffer == 1 ? 0 : 1);
        }
    }
}


/**
 * @brief Repeatedly flash the Pico's built-in LED.
 */
void led_task_pico(void* unused_arg) {
    // Store the Pico LED state
    uint8_t pico_led_state = 0;
    DEBUG("BEGIN led_task_pico");
    
    // Configure the Pico's on-board LED
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    
    while (true) {
        // Turn Pico LED on an add the LED state
        // to the FreeRTOS xQUEUE
        DEBUG("PICO LED FLASH");
        pico_led_state = 1;
        gpio_put(PICO_DEFAULT_LED_PIN, pico_led_state);
        xQueueSendToBack(queue, &pico_led_state, 0);
        vTaskDelay(ms_delay);
        
        // Turn Pico LED off an add the LED state
        // to the FreeRTOS xQUEUE
        pico_led_state = 0;
        gpio_put(PICO_DEFAULT_LED_PIN, pico_led_state);
        xQueueSendToBack(queue, &pico_led_state, 0);
        vTaskDelay(ms_delay);
    }
}

class Plinky : public Task 
{
public:
    Plinky(const char *name, QueueHandle_t q=NULL)
	:Task("Plinky",name,q, sizeof(uint8_t))
    {
	DEBUG("Plinky constructor");
    }
    
    virtual void loop () {
	DEBUG("Plinky loop");
	led_task_pico(NULL);
    }
};

class Glinky : public Task 
{
public:
    Glinky(const char *name, QueueHandle_t q=NULL)
	:Task("Glinky",name,q, sizeof(uint8_t))
    {
	DEBUG("Glinky constructor");
    }
    
    virtual void loop () {
	DEBUG("Glinky loop");
	led_task_gpio(NULL);
    }
};


/*
 * RUNTIME START
 */
int main() {
    stdio_init_all();
    sleep_ms(5000);

    // Log app info
    log_device_info();

    NOTICE("Create queue");
    queue = xQueueCreate(4, sizeof(uint8_t));
    
    NOTICE("Create tasks");
    Task *pico_task = new Plinky("pico_task", queue);
    Task *gpio_task = new Glinky("gpio_task", queue);
    
    NOTICE("Start Scheduler");
    vTaskStartScheduler();
    
    NOTICE("NOTREACHED");
    while(true); 
}
