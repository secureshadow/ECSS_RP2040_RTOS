#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "FreeRTOS.h"
#include "FreeRTOSConfig.h"
#include "task.h"
#include "hardware/i2c.h"
#include <queue.h>


#define I2C_PORT i2c0
#define BMP180_ADDR 0x77 // Replace with your sensor's I2C address
#define SDA_PIN 4        // Replace with your actual SDA pin number
#define SCL_PIN 5        // Replace with your actual SCL pin number
#define REG_ID 0xD0      // BMP180 ID register (can be used to check communication)

void i2c_communication_task(void *params) {
    uint8_t buf[10]; // Buffer to store the data from BMP180
    
    // Initialize I2C
    i2c_init(I2C_PORT, 100 * 1000);
    gpio_set_function(SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(SDA_PIN);
    gpio_pull_up(SCL_PIN);

    while (1) {
        buf[0] = REG_ID; // Register to read from
        
        i2c_write_blocking(I2C_PORT, BMP180_ADDR, buf, 1, true); // Set the register pointer
        i2c_read_blocking(I2C_PORT, BMP180_ADDR, buf, sizeof(buf), false); // Read multiple bytes

        // Print the raw values
        printf("Data read: ");
        for (int i = 0; i < sizeof(buf); i++) {
            printf("0x%x ", buf[i]);
        }
        printf("\n");

        vTaskDelay(pdMS_TO_TICKS(1000));
    }
}

int main() {
    stdio_init_all();
    xTaskCreate(i2c_communication_task, "I2C_Communication_Task", 256, NULL, 1, NULL);
    vTaskStartScheduler();
    while (1) {}
}
