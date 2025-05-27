void write_str(const char* str, unsigned short row, unsigned short col, unsigned char color) {
    volatile unsigned short* vga = (unsigned short*)0xB8000;
    unsigned short offset = row * 80 + col;
    while(*str) {
        vga[offset++] = (color << 8) | *str++;
    }
}

void kernel_main() {
    write_str("Hello from kernel!", 10, 30, 0x0F);
    for (;;);
}