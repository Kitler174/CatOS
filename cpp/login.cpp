#include <stdint.h>
#include "../h/framebuffer.h"
#include "../h/input.h"
#include "../h/mouse.h"
#include "../h/structs.h"

int screen_w, screen_h;

void login(uint32_t magic, uint32_t mb2_addr) {
    uint8_t* addr = (uint8_t*)mb2_addr;
    uint32_t total_size = *(uint32_t*)addr;
    addr += 8;
    while (true) {
        multiboot2_tag* tag = (multiboot2_tag*)addr;
        if (tag->type == 0) break; // end tag

        if (tag->type == MULTIBOOT2_TAG_TYPE_FRAMEBUFFER) {
            uint64_t fb_addr = *(uint64_t*)(addr + 8);
            uint32_t fb_pitch = *(uint32_t*)(addr + 16);
            uint32_t fb_width = *(uint32_t*)(addr + 20);
            uint32_t fb_height = *(uint32_t*)(addr + 24);
            uint8_t  fb_bpp = *(uint8_t*)(addr + 28);
            uint32_t* fb = (uint32_t*)(uintptr_t)fb_addr;

            // Ustaw rozmiar ekranu dla myszy!
            screen_w = fb_width;
            screen_h = fb_height;

            draw_rect(fb, fb_width, fb_height, 0, 0, fb_width, fb_height, 0x000000FF);
            draw_rect(fb, fb_width, fb_height, fb_width/2-256, fb_height/2-256, 512, 512, 0x000000);
            draw_rect(fb, fb_width, fb_height, fb_width/2-178, fb_height/2-150, 356, 50, 0xFFFFFF);
            draw_rect(fb, fb_width, fb_height, fb_width/2-178, fb_height/2-75, 356, 50, 0xFFFFFF);

            if (fb_bpp == 32) {
                int old_x = mouse_x, old_y = mouse_y;
                while (1) {
                    // Wyczyść starą pozycję kursora (zamaluj na kolor tła)
                    draw_rect(fb, fb_width, fb_height, old_x-1, old_y-1, 3, 3, 0x000000);

                    mouse_poll();

                    // Narysuj nowy kursor
                    mouse_draw_cursor(fb, fb_width, fb_height, 0x00FF0000);

                    old_x = mouse_x;
                    old_y = mouse_y;
                }
            }
        }
        addr += (tag->size + 7) & ~7;
    }
}