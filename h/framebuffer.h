#pragma once
#include <stdint.h>

// font8x8_basic jest zdefiniowane tylko raz w 8x8.cpp lub framebuffer.cpp!
extern uint8_t font8x8_basic[128][8];

// Rysowanie pojedynczego znaku
void draw_char8x8(uint32_t* fb, int fb_width, int fb_height, int x, int y, uint8_t c, uint32_t color);

// Rysowanie napisu
void draw_string8x8(uint32_t* fb, int fb_width, int fb_height, int x, int y, const char* str, uint32_t color);

// Rysowanie prostokąta
void draw_rect(uint32_t* fb, int fb_width, int fb_height, int x, int y, int w, int h, uint32_t color);