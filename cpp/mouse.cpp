#include "../h/input.h"
#include "../h/framebuffer.h"

int mouse_x = 0, mouse_y = 0;
int prev_mouse_x = 0, prev_mouse_y = 0;
int dx, dy;
uint8_t mouse_buttons = 0;
int cursor_initialized = 0; 
static int screen_w = 1024, screen_h = 768;
static uint32_t saved_background[18 * 18]; // tło kursora

// --- Konwersja liczby na string ---
void itoa(int value, char* buffer, int base) {
    char* ptr = buffer;
    char* ptr1;
    char tmp_char;
    int is_negative = 0;

    if (value < 0 && base == 10) {
        is_negative = 1;
        value = -value;
    }

    do {
        int remainder = value % base;
        *ptr++ = (remainder < 10) ? (remainder + '0') : (remainder - 10 + 'a');
    } while (value /= base);

    if (is_negative)
        *ptr++ = '-';

    *ptr = '\0';

    ptr1 = buffer;
    char* end = ptr - 1;
    while (ptr1 < end) {
        tmp_char = *ptr1;
        *ptr1++ = *end;
        *end-- = tmp_char;
    }
}

// --- Inicjalizacja myszy ---
void mouse_init() {
    outb(0x64, 0xA8); // włącz urządzenie pomocnicze
    outb(0x64, 0x20);
    uint8_t status = inb(0x60) | 2;
    outb(0x64, 0x60);
    outb(0x60, status);

    outb(0x64, 0xD4);
    outb(0x60, 0xF4);
    inb(0x60); // ACK
}

void mouse_draw_cursor(uint32_t* fb, int fb_width, int fb_height, uint32_t color) {
    int radius = 8;
    int inner_radius = 4;

    // Jeśli bufor tła jest już zainicjalizowany, to przywracamy stare tło
    if (cursor_initialized) {
        for (int dy = -radius; dy <= radius; dy++) {
            for (int dx = -radius; dx <= radius; dx++) {
                int px = prev_mouse_x + dx;
                int py = prev_mouse_y + dy;
                if (dx*dx + dy*dy <= radius*radius && px >= 0 && px < fb_width && py >= 0 && py < fb_height) {
                    fb[py * fb_width + px] = saved_background[(dy + radius) * (2 * radius + 1) + (dx + radius)];
                }
            }
        }
    }

    // Zapisz tło pod nową pozycją
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int px = mouse_x + dx;
            int py = mouse_y + dy;
            if (dx*dx + dy*dy <= radius*radius && px >= 0 && px < fb_width && py >= 0 && py < fb_height) {
                saved_background[(dy + radius) * (2 * radius + 1) + (dx + radius)] = fb[py * fb_width + px];
            }
        }
    }

    // Zaznacz, że bufor tła został zainicjalizowany
    cursor_initialized = 1;

    // Rysuj kursor (okrągły, z pustym środkiem)
    for (int dy = -radius; dy <= radius; dy++) {
        for (int dx = -radius; dx <= radius; dx++) {
            int r2 = dx*dx + dy*dy;
            if (r2 <= radius*radius && r2 >= inner_radius*inner_radius) {
                int px = mouse_x + dx;
                int py = mouse_y + dy;
                if (px >= 0 && px < fb_width && py >= 0 && py < fb_height) {
                    fb[py * fb_width + px] = color;
                }
            }
        }
    }

    // Debug info
    char buffer[12];
    draw_rect(fb, fb_width, fb_height, 8, 8, 80, 56, 0x00000000);
    itoa(mouse_x, buffer, 10);
    draw_string8x8(fb, fb_width, fb_height, 8, 8, "mouse:", 0xFFFFFF);
    draw_string8x8(fb, fb_width, fb_height, 56, 8, buffer, 0xFFFFFF);
    itoa(mouse_y, buffer, 10);
    draw_string8x8(fb, fb_width, fb_height, 56, 24, buffer, 0xFFFFFF);
    draw_string8x8(fb, fb_width, fb_height, 8, 40, "dx:", 0xFFFFFF);
    itoa(dx, buffer, 10);
    draw_string8x8(fb, fb_width, fb_height, 56, 40, buffer, 0xFFFFFF);
    draw_string8x8(fb, fb_width, fb_height, 8, 56, "dy:", 0xFFFFFF);
    itoa(dy, buffer, 10);
    draw_string8x8(fb, fb_width, fb_height, 56, 56, buffer, 0xFFFFFF);

    prev_mouse_x = mouse_x;
    prev_mouse_y = mouse_y;
}

// --- Obsługa pakietów myszy ---
void mouse_poll(uint32_t* fb, int fb_width, int fb_height, uint32_t color) {
    static uint8_t packet[3];
    static int packet_idx = 0;

    // Przetwarzaj wszystkie dostępne bajty z bufora myszy
    while (inb(0x64) & 1) {
        uint8_t byte = inb(0x60);

        // Synchronizacja pakietu
        if (packet_idx == 0 && (byte & 0x08) == 0) {
            continue; // pomiń niesynchronizowany bajt
        }

        packet[packet_idx++] = byte;

        if (packet_idx == 3) {
            dx = (int8_t)packet[1];
            dy = (int8_t)packet[2];

            if (packet[0] & 0x40) dx = 0; // overflow X
            if (packet[0] & 0x80) dy = 0; // overflow Y

            mouse_buttons = packet[0] & 0x07;

            // Aktualizacja pozycji kursora
            mouse_x += dx;
            mouse_y -= dy; // odwrócenie osi Y

            if (mouse_x < 9) mouse_x = 9;
            else if (mouse_x+9 >= screen_w) mouse_x = screen_w - 9;
            if (mouse_y < 9) mouse_y = 9;
            else if (mouse_y+9 >= screen_h) mouse_y = screen_h - 9;

            // Odśwież kursor tylko przy ruchu
            if (mouse_x != prev_mouse_x || mouse_y != prev_mouse_y) {
                mouse_draw_cursor(fb, fb_width, fb_height, color);
            }

            packet_idx = 0;
        }
    }
}
