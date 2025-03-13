#include "gradient/gradient.h"
#include "ili9341/ili9341.h"
#include "math.h"  

uint16_t rgb2pix(uint8_t r, uint8_t g, uint8_t b){
    //uint8_t short_r = (r / 255) * 32;
    //uint8_t short_g = (g / 255) * 64;
    uint8_t short_b = (b / 255) * 32;
    return short_b;
}

static void inline draw_line(uint16_t color, uint16_t line, uint8_t* buffer)
{
    for (int i = line * SCREEN_WIDTH * 2; i < (line + 1) * SCREEN_WIDTH * 2; i+=2)
    {
        buffer[i] = color;
        buffer[i+1] = color;
    }
}

void handle_gradient(uint8_t* buffer) {
    //uint16_t end_color = rgb2pix(0, 0, 255);
    uint16_t end_color = 320;
    uint16_t color_factor = end_color / SCREEN_HEIGHT;
    for (int line = 0; line < SCREEN_HEIGHT; line++) {      
        draw_line(color_factor * line, line, buffer);
    }
}
