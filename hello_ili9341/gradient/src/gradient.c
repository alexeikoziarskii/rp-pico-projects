#include "gradient/gradient.h"
#include "ili9341/ili9341.h"
#include "math.h"  
#define FADE_STEPS 50
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 320
#define INITIAL_BRIGHTNESS 255

uint16_t convert_to_16bit(uint8_t r, uint8_t g, uint8_t b) {
    return ((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3);
}

void draw_frame(uint16_t color, uint8_t brightness, uint8_t* buffer) {
    uint8_t r = (((color >> 11) & 0x1F) * 255 / 31) * brightness / 255;
    uint8_t g = (((color >> 5) & 0x3F) * 255 / 63) * brightness / 255;
    uint8_t b = ((color & 0x1F) * 255 / 31) * brightness / 255;
    uint16_t new_color = convert_to_16bit(r, g, b);
    
    for (int i = 0; i < SCREEN_HEIGHT; i++) {
        for (int j = 0; j < SCREEN_WIDTH; j++) {
            int index = (i * SCREEN_WIDTH + j) * 2;
            buffer[index] = new_color >> 8;
            buffer[index + 1] = new_color & 0xFF;
        }
    }
}

void handle_gradient(const uint16_t* colors, int color_count, uint8_t* buffer) {
    for (int i = 0; i < color_count - 1; i++) {
        for (int step = 0; step <= FADE_STEPS; step++) {
            float factor = (float)step / FADE_STEPS;
            uint16_t start_color = colors[i];
            uint16_t end_color = colors[i + 1];
            
            uint8_t start_r = ((start_color >> 11) & 0x1F) * 255 / 31;
            uint8_t start_g = ((start_color >> 5) & 0x3F) * 255 / 63;
            uint8_t start_b = (start_color & 0x1F) * 255 / 31;
            
            uint8_t end_r = ((end_color >> 11) & 0x1F) * 255 / 31;
            uint8_t end_g = ((end_color >> 5) & 0x3F) * 255 / 63;
            uint8_t end_b = (end_color & 0x1F) * 255 / 31;
            
            uint8_t r = start_r + (uint8_t)((end_r - start_r) * factor);
            uint8_t g = start_g + (uint8_t)((end_g - start_g) * factor);
            uint8_t b = start_b + (uint8_t)((end_b - start_b) * factor);
            
            uint16_t interpolated_color = convert_to_16bit(r, g, b);
            uint8_t brightness = INITIAL_BRIGHTNESS * (1.0 - factor);
            draw_frame(interpolated_color, brightness, buffer);
        }
    }
}
