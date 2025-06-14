#pragma once
#include <stdint.h>

// Stała do rozpoznawania typu tagu framebuffer w Multiboot2
#define MULTIBOOT2_TAG_TYPE_FRAMEBUFFER 8

// Typ funkcji konstruktora (np. dla globalnych obiektów C++)
typedef void (*constructor)();

// Struktura Multiboot1 info (rozszerzona o framebuffer)
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

// Struktura tagu Multiboot2 (np. do parsowania tagów)
struct multiboot2_tag {
    uint32_t type;
    uint32_t size;
};