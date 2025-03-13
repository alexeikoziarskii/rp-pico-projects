#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ili9341/ili9341.h"
#include "gradient/gradient.h"
#include "stdlib.h"

#define COLOR_COUNT 4 

// SPI Defines
// We are going to use SPI 0, and allocate it to the following GPIO pins
// Pins can be changed, see the GPIO function select table in the datasheet for information on GPIO assignments


int main()
{
    stdio_init_all();

    init_SPI();
    init_display();
    init_drawing();


    const uint16_t colors[] = {
        0xF800,  // Красный
        0x07E0,  // Зеленый
        0x001F,  // Синий
        0xFFFF   // Белый
    };
    

    while (true) {
        clear_buffer();
        handle_gradient(colors, COLOR_COUNT, get_buffer());
        display_buffer();
        // display_external_buffer(get_antlab_image_buffer());
    }
}