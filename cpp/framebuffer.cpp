#pragma once
#include <stdint.h>
#include "../h/8x8.h"
#include "../h/framebuffer.h"

void draw_char8x8(uint32_t* fb, int fb_width, int fb_height, int x, int y, uint8_t c, uint32_t color) {
    if ((unsigned char)c > 127) return;
    for (int row = 0; row < 8; ++row) {
        unsigned char bits = font8x8_basic[(uint8_t)c][row];
        for (int col = 0; col < 8; ++col) {
            if (bits & (1 << col)){
                int px = x + col;
                int py = y + row;
                if (px >= 0 && px < fb_width && py >= 0 && py < fb_height) {
                    fb[py * fb_width + px] = color;
                }
            }
        }
    }
}

void draw_string8x8(uint32_t* fb, int fb_width, int fb_height, int x, int y, const char* str, uint32_t color) {
    int cx = x;
    while (*str) {
        draw_char8x8(fb, fb_width, fb_height, cx, y, *str, color);
        cx += 8;
        ++str;
    }
}

void draw_rect(uint32_t* fb, int fb_width, int fb_height, int x, int y, int w, int h, uint32_t color) {
    for (int iy = y; iy < y + h && iy < fb_height; ++iy) {
        for (int ix = x; ix < x + w && ix < fb_width; ++ix) {
            fb[iy * fb_width + ix] = color;
        }
    }
}
