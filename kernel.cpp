#include <stdint.h>
#include "8x8.h"

typedef void (*constructor)();
extern "C" constructor* start_ctors;
extern "C" constructor* end_ctors;
extern "C" void callConstructors()
{
    for(constructor* i=start_ctors;i!=end_ctors;i++){
        (*i)();
    }
}

struct multiboot_info_t {
    uint32_t flags;
    uint32_t mem_lower;
    uint32_t mem_upper;
    uint32_t boot_device;
    uint32_t cmdline;
    uint32_t mods_count;
    uint32_t mods_addr;
    uint32_t syms[4];
    uint32_t mmap_length;
    uint32_t mmap_addr;
    uint32_t drives_length;
    uint32_t drives_addr;
    uint32_t config_table;
    uint32_t boot_loader_name;
    uint32_t apm_table;
    uint32_t vbe_control_info;
    uint32_t vbe_mode_info;
    uint16_t vbe_mode;
    uint16_t vbe_interface_seg;
    uint16_t vbe_interface_off;
    uint16_t vbe_interface_len;

    // Framebuffer (od offsetu 0x34)
    uint64_t framebuffer_addr;
    uint32_t framebuffer_pitch;
    uint32_t framebuffer_width;
    uint32_t framebuffer_height;
    uint8_t  framebuffer_bpp;
    uint8_t  framebuffer_type;
    uint8_t  reserved;
};
void draw_rect(uint32_t* fb, int fb_width, int fb_height, int x, int y, int w, int h, uint32_t color) {
    for (int iy = y; iy < y + h && iy < fb_height; ++iy) {
        for (int ix = x; ix < x + w && ix < fb_width; ++ix) {
            fb[iy * fb_width + ix] = color;
        }
    }
}

void printf(const char* str)
{
    static unsigned short* VideoMemory = (unsigned short*)0xb8000;
    static int cursor = 0;
    for(int i=0; str[i]!='\0'; ++i){
        VideoMemory[cursor++] = (VideoMemory[cursor] & 0xFF00) | str[i];
        if (str[i] == '\n') {
            cursor += 80 - (cursor % 80); // przejdź do nowej linii
        }
    }
}
void print_hex8(uint8_t val) {
    char buf[5] = "0x00";
    for(int i = 0; i < 2; ++i) {
        int shift = (1 - i) * 4;
        uint8_t digit = (val >> shift) & 0xF;
        buf[2 + i] = digit < 10 ? '0' + digit : 'A' + (digit - 10);
    }
    buf[4] = '\0';
    printf(buf);
}
void print_hex(uint32_t val) {
    char buf[11] = "0x00000000";
    for(int i = 0; i < 8; ++i) {
        int shift = (7 - i) * 4;
        uint8_t digit = (val >> shift) & 0xF;
        buf[2 + i] = digit < 10 ? '0' + digit : 'A' + (digit - 10);
    }
    printf(buf);
}
struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};

#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8





static inline void draw_char8x8(uint32_t* fb, int fb_width, int fb_height, int x, int y, uint8_t c, uint32_t color) {
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

static inline void draw_string8x8(uint32_t* fb, int fb_width, int fb_height, int x, int y, const char* str, uint32_t color) {
    int cx = x;
    while (*str) {
        draw_char8x8(fb, fb_width, fb_height, cx, y, *str, color);
        cx += 8;
        ++str;
    }
}

extern "C" void kernelMain(uint32_t magic, uint32_t mb2_addr) {
    uint8_t* addr = (uint8_t*)mb2_addr;
    uint32_t total_size = *(uint32_t*)addr;
    addr += 8; // skip total_size and reserved

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
                draw_string8x8(fb, fb_width, fb_height, 8, 8, "Witaj w CatOS!", 0x00FFFFFF); // czerwony kwadrat
                draw_string8x8(fb, fb_width, fb_height, 8, 24, "Projekt stworzony przez Kitler174", 0x00FFFFFF); // czerwony kwadrat
                draw_string8x8(fb, fb_width, fb_height, 8, 40, "Wcisnij enter aby kontynuowac instalacje...", 0x00FF0000); // czerwony kwadrat
                draw_rect(fb,fb_width, fb_height, 0, fb_height-48,fb_width, 48, 0x00FFFFFF);
                draw_string8x8(fb, fb_width, fb_height, fb_width/2-144, fb_height - 28, "github: https://github.com/Kitler174", 0x00000000); // czerwony kwadrat

                
                while(1);
            }
        }
        // przejdź do kolejnego taga (wyrównanie do 8 bajtów)
        addr += (tag->size + 7) & ~7;
    }

    // Jeśli nie znaleziono tagu framebuffer, zatrzymaj się
    while(1);
}
