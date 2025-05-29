#include "../h/input.h" // dla inb, outb

int mouse_x = 100, mouse_y = 100;
uint8_t mouse_buttons = 0;

static int screen_w = 640, screen_h = 480;

void mouse_draw_cursor(uint32_t* fb, int fb_width, int fb_height, uint32_t color) {
    // Prosty krzyżyk jako kursor
    for (int dx = -1; dx <= 1; dx++)
        for (int dy = -1; dy <= 1; dy++) {
            int px = mouse_x + dx, py = mouse_y + dy;
            if (px >= 0 && px < fb_width && py >= 0 && py < fb_height)
                fb[py * fb_width + px] = color;
        }
}

void mouse_init() {
    // Aktywuj mysz PS/2 (prosty tryb)
    outb(0x64, 0xA8); // enable auxiliary device
    outb(0x64, 0x20); // get Compaq status byte
    uint8_t status = (inb(0x60) | 2);
    outb(0x64, 0x60); // set Compaq status byte
    outb(0x60, status);

    // Włącz raportowanie danych
    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    inb(0x60); // Odczytaj ACK
}

void mouse_poll() {
    static uint8_t packet[3];
    static int packet_idx = 0;

    // Sprawdź, czy jest coś w PS/2 port 0x60
    if ((inb(0x64) & 1) == 0) return;

    packet[packet_idx++] = inb(0x60);

    if (packet_idx == 3) {
        int dx = ((int8_t)packet[1]);
        int dy = ((int8_t)packet[2]);

        mouse_x += dx;
        mouse_y -= dy; // kierunek Y odwrotny

        // Granice ekranu
        if (mouse_x < 0) mouse_x = 0;
        if (mouse_y < 0) mouse_y = 0;
        if (mouse_x >= screen_w) mouse_x = screen_w - 1;
        if (mouse_y >= screen_h) mouse_y = screen_h - 1;

        mouse_buttons = packet[0] & 0x07;

        packet_idx = 0;
    }
}