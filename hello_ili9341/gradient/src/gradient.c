#include "gradient/gradient.h"

#include "ili9341/ili9341.h"

#include "math.h"  
#define SCREEN_WIDTH 320
#define PI 3.14159265

static void inline draw_sin_gradient(uint16_t start_color, uint16_t end_color, uint8_t* buffer)
{
    for (int i = 0; i < SCREEN_HEIGHT; i++) 
    {
        for (int j = 0; j < SCREEN_WIDTH; j++) 
        {
            float factor = (sin((float)(i + j) / (SCREEN_WIDTH + SCREEN_HEIGHT) * PI) + 1) / 2; // Нормализация в диапазон [0, 1]
            uint8_t color = start_color + (uint8_t)((end_color - start_color) * factor);
            
            int index = SCREEN_WIDTH;
            buffer[index] = color;
            buffer[index + 1] = color;
        }
    }
}


void handle_sin_gradient(uint8_t* buffer)
{
    draw_sin_gradient(0, 0xFFFF, buffer);
}
