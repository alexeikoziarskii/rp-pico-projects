#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "ili9341/ili9341.h"
#include "ant-lab/ant-lab.h"
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

    //clear_buffer();
    //display_external_buffer(get_antlab_image_buffer());

    /*while (true) {
        clear_buffer();
        handle_gradient(get_buffer());
        display_buffer();
    }*/
}