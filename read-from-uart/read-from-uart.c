#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdint.h>
#include "pico/stdlib.h"
#include "hardware/uart.h"

#define BAUD_RATE 115200      // Скорость передачи
#define MAX_CMD_LENGTH 32     // Максимальная длина команды
#define MAX_TIMER_LENGTH 10     // Максимальная длина таймера
#define MAX_ADDR_LENGTH 11    // Максимальная длина адреса (для ввода)

#ifdef CYW43_WL_GPIO_LED_PIN
#include "pico/cyw43_arch.h"
#endif

#ifndef LED_DELAY_MS
#define LED_DELAY_MS 1000
#endif

// Perform initialisation
int pico_led_init(void) {
#if defined(PICO_DEFAULT_LED_PIN)
    // A device like Pico that uses a GPIO for the LED will define PICO_DEFAULT_LED_PIN
    // so we can use normal GPIO functionality to turn the led on and off
    gpio_init(PICO_DEFAULT_LED_PIN);
    gpio_set_dir(PICO_DEFAULT_LED_PIN, GPIO_OUT);
    return PICO_OK;
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // For Pico W devices we need to initialise the driver etc
    return cyw43_arch_init();
#endif
}

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

// Функция для чтения 4 байт из памяти по адресу
void read_memory(uint32_t addr) {
    if (addr >= 0x10000000 && addr < 0x10200000) {  // Диапазон памяти Flash
        printf("Address 0x%08lX is in Flash memory range.\n", addr);
    } else if (addr >= 0x20000000 && addr < 0x20040000) {  // Диапазон памяти SRAM
        printf("Address 0x%08lX is in SRAM memory range.\n", addr);
    } else {
        printf("Address 0x%08lX is out of range.\n", addr);
        return;
    }

    uint32_t *ptr = (uint32_t *)addr;  // Преобразуем адрес в указатель на 32-битное значение
    uint32_t value = *ptr;  // Читаем 32-битное значение из памяти

    printf("Memory value at 0x%08lX: 0x%08X\n", addr, value);
}

// Turn the led on or off
void pico_set_led(bool led_on) {
#if defined(PICO_DEFAULT_LED_PIN)
    // Just set the GPIO on or off
    gpio_put(PICO_DEFAULT_LED_PIN, led_on);
#elif defined(CYW43_WL_GPIO_LED_PIN)
    // Ask the wifi "driver" to set the GPIO on or off
    cyw43_arch_gpio_put(CYW43_WL_GPIO_LED_PIN, led_on);
#endif
}

int main() {
    stdio_init_all();  // Инициализация стандартного вывода
    int rc = pico_led_init();
    hard_assert(rc == PICO_OK);

    char command[MAX_CMD_LENGTH];  // Буфер для команды

    char time[MAX_TIMER_LENGTH];   // Буфер для таймера
    char* endptr;

    char addr_str[MAX_ADDR_LENGTH]; // Буфер для адреса
    uint32_t addr;

    while (true) {
        printf("enter new command\n");
        read_uart_string(command, MAX_CMD_LENGTH);  // Читаем строку из UART

        if (strcmp(command, "enable led") == 0) {  // Если команда "enable"
            printf("set duration(ms)\n");
            read_uart_string(time, MAX_TIMER_LENGTH);
            printf("Received: [%s]\n", time); 

            int time_ms = strtol(time, &endptr, 10);
            if (*endptr == '\0' || *endptr == '\n') {
                // Если число было успешно преобразовано
                printf("timer has been set for %ldms\n", time_ms);
            } else {
                // Если строка не является числом
                printf("not a number\n");
            }

            pico_set_led(true);
            printf("LED ON\n");

            sleep_ms(time_ms); 

            pico_set_led(false);
            printf("LED OFF\n");

        } 

        else if (strcmp(command, "readmem") == 0) {  // Если команда "readmem"
            printf("Enter memory address to read (in hex):\n");
            read_uart_string(addr_str, MAX_ADDR_LENGTH);  // Читаем строку с адресом

            addr = strtol(addr_str, &endptr, 16);  // Преобразуем строку в число в шестнадцатеричной системе

            if (*endptr == '\0' || *endptr == '\n') {
                read_memory(addr);  // Читаем данные из памяти по адресу
            } else {
                printf("Invalid address format.\n");
            }
        } 
        else {
            printf("Unknown command: %s\n", command);
        }

        sleep_ms(10);  // Задержка для экономии ресурсов
    } 
}
