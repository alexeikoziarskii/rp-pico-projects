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


 //чтение строки из uart
void read_uart_string(char *buffer, size_t max_length) {
    size_t index = 0;
    while (index < max_length - 1) {
        char ch = getchar();  // Читаем символ
        if (ch == '\r' || ch == '\n') {  // Конец строки (Enter)
            buffer[index] = '\0';  // Завершаем строку
            return;
        }
        buffer[index++] = ch;  // Добавляем в буфер
    }
    buffer[max_length - 1] = '\0';  // Защита от переполнения
}

int main() {
    stdio_init_all();
    printf("ADC Example, measuring GPIO26\n");
 
     adc_init();
 
     // Make sure GPIO is high-impedance, no pullups etc
     adc_gpio_init(26);
     // Select ADC input 0 (GPIO26)
     adc_select_input(0);
 
    char command[MAX_CMD_LENGTH];  // Буфер для команды
    char time[MAX_TIMER_LENGTH];   // Буфер для таймера

    while(true){
        read_uart_string(command, MAX_CMD_LENGTH);
        if(strcmp(command, "sample") == 0){
            const float conversion_factor = 3.3f / (1 << 12);
            uint16_t result = adc_read();
            sleep_ms(500);
            printf("%f\n", result * conversion_factor);
        }
    }   
}
 