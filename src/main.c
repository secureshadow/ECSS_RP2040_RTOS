#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "hardware/i2c.h"
#include <queue.h>

void blinkled_task(void *params) {
    const uint LED_PIN = 25;
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

    while (1) {
        gpio_put(LED_PIN, 1); // Turn the LED ON
        vTaskDelay(pdMS_TO_TICKS(1000)); // Wait for 1000 ms
        gpio_put(LED_PIN, 0); // Turn the LED OFF
        vTaskDelay(pdMS_TO_TICKS(500)); // Wait for another 500 ms
    }
}

int main() {
    stdio_init_all();
    xTaskCreate(blinkled_task, "BLINKLED_TASK", 256, NULL, 1, NULL);
    vTaskStartScheduler();

    while (1) {};
}