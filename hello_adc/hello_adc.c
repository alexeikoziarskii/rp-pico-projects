/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "hardware/gpio.h"
#include "hardware/adc.h"
 
#define BAUD_RATE 115200      // Скорость передачи
#define MAX_CMD_LENGTH 32     // Максимальная длина команды
#define MAX_TIMER_LENGTH 10     // Максимальная длина таймера
#define GPIO_PIN 15 
#define VOLTAGE_HIGHER_THRESHOLD 3.2f
#define VOLTAGE_LOWER_THRESHOLD 0.015f

 //чтение строки из uart
 char read_uart_char() {
    while (!stdio_usb_connected());  // Ожидание подключения USB-UART
    return getchar();
}

int main() {
    stdio_init_all();
    gpio_init(GPIO_PIN);
    gpio_set_dir(GPIO_PIN, GPIO_OUT);

    printf("ADC Example, measuring GPIO26\n");
 
     adc_init();
 
     // Make sure GPIO is high-impedance, no pullups etc
     adc_gpio_init(26);
     // Select ADC input 0 (GPIO26)
     adc_select_input(0);
 
    char command[MAX_CMD_LENGTH];  // Буфер для команды
    char time[MAX_TIMER_LENGTH];   // Буфер для таймера
    const float conversion_factor = 3.3f / (1 << 12);
    bool gpio_state = false; 
    bool charge_flag = 1;
    float voltage;

    while(true){

        uint16_t result = adc_read();
        voltage = result * conversion_factor;
        
        printf("%f\n", result * conversion_factor);

        if(!gpio_state && voltage <= VOLTAGE_LOWER_THRESHOLD){
            gpio_put(GPIO_PIN, 1);
            printf("GPIO %d -> HIGH\n", GPIO_PIN);
            gpio_state = true;
        }
        if (gpio_state && voltage >= VOLTAGE_HIGHER_THRESHOLD) {
            gpio_put(GPIO_PIN, 0);
            gpio_state = false;
            printf("Достигнут порог %.2fV! GPIO выключен (LOW)\n", VOLTAGE_HIGHER_THRESHOLD);
        }

        sleep_ms(2);
    }   
}
 