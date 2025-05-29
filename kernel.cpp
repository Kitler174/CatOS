#include <stdint.h>
#include "h/framebuffer.h"
#include "h/structs.h"
#include "h/input.h"
#include "h/login.h"

extern "C" {
    extern constructor* start_ctors;
    extern constructor* end_ctors;
    void callConstructors();
}

extern "C" void kernelMain(uint32_t magic, uint32_t mb2_addr) {
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

            if (fb_bpp == 32) {
                uint32_t* fb = (uint32_t*)(uintptr_t)fb_addr;
                draw_string8x8(fb, fb_width, fb_height, 8, 8, "Witaj w CatOS!", 0x00FFFFFF);
                draw_string8x8(fb, fb_width, fb_height, 8, 24, "Projekt stworzony przez Kitler174", 0x00FFFFFF);
                draw_string8x8(fb, fb_width, fb_height, 8, 40, "Wcisnij enter aby sie zalogowac...", 0x00FF0000); 
                draw_rect(fb,fb_width, fb_height, 0, fb_height-48,fb_width, 48, 0x00FFFFFF);
                draw_string8x8(fb, fb_width, fb_height, fb_width/2-144, fb_height - 28, "github: https://github.com/Kitler174", 0x00000000);
            }
            
            while (inb(0x60) != 0x1C){}
            login(magic, mb2_addr);
        }
        addr += (tag->size + 7) & ~7;
    }
    while(1);
}
