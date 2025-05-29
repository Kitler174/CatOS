#pragma once
#include <stdint.h>

void mouse_init();
void mouse_poll();
void mouse_draw_cursor(uint32_t* fb, int fb_width, int fb_height, uint32_t color);

extern int mouse_x, mouse_y;
extern uint8_t mouse_buttons;