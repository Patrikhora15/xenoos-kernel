#ifndef VGA_H
#define VGA_H

#include "common.h"

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_ADDRESS 0xB8000

enum VGAColor {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

uint16_t* vga_buffer = (uint16_t*)VGA_ADDRESS;
size_t vga_row = 0;
size_t vga_column = 0;
uint8_t vga_color = VGA_COLOR_LIGHT_GREY | (VGA_COLOR_BLACK << 4);

uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | (uint16_t)color << 8;
}

void vga_set_color(uint8_t fg, uint8_t bg) {
    vga_color = fg | (bg << 4);
}

void vga_putc_at(char c, size_t x, size_t y) {
    if (x >= VGA_WIDTH || y >= VGA_HEIGHT) return;
    vga_buffer[y * VGA_WIDTH + x] = vga_entry(c, vga_color);
}

void vga_puts(const char* str);
void vga_clear();

size_t vga_strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

void vga_puts_at(size_t x, size_t y, const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        if (x + i >= VGA_WIDTH) break;
        vga_putc_at(str[i], x + i, y);
    }
}

void vga_puts_color(const char* str, uint8_t fg, uint8_t bg) {
    uint8_t prev = vga_color;
    vga_set_color(fg, bg);
    vga_puts(str);
    vga_color = prev;
}

void vga_fill_rect(size_t x0, size_t y0, size_t width, size_t height, uint8_t color) {
    uint8_t prev = vga_color;
    vga_set_color(color & 0x0F, color >> 4);
    for (size_t y = y0; y < y0 + height; y++) {
        for (size_t x = x0; x < x0 + width; x++) {
            if (x < VGA_WIDTH && y < VGA_HEIGHT) {
                vga_putc_at(' ', x, y);
            }
        }
    }
    vga_color = prev;
}

void vga_draw_box(size_t x0, size_t y0, size_t width, size_t height, uint8_t color) {
    uint8_t prev = vga_color;
    vga_set_color(color & 0x0F, color >> 4);
    if (width < 2 || height < 2) {
        vga_color = prev;
        return;
    }
    size_t x1 = x0 + width - 1;
    size_t y1 = y0 + height - 1;
    vga_putc_at('+', x0, y0);
    vga_putc_at('+', x1, y0);
    vga_putc_at('+', x0, y1);
    vga_putc_at('+', x1, y1);

    for (size_t x = x0 + 1; x < x1; x++) {
        vga_putc_at('-', x, y0);
        vga_putc_at('-', x, y1);
    }
    for (size_t y = y0 + 1; y < y1; y++) {
        vga_putc_at('|', x0, y);
        vga_putc_at('|', x1, y);
    }
    vga_color = prev;
}

void vga_print_centered(size_t y, const char* str) {
    size_t len = vga_strlen(str);
    size_t x = len >= VGA_WIDTH ? 0 : (VGA_WIDTH - len) / 2;
    vga_puts_at(x, y, str);
}

void vga_draw_panic_screen(const char* title, const char* message) {
    vga_clear();
    uint8_t prev = vga_color;
    uint8_t panic_color = VGA_COLOR_WHITE | (VGA_COLOR_RED << 4);
    vga_fill_rect(0, 0, VGA_WIDTH, VGA_HEIGHT, panic_color);
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_print_centered(1, title);
    vga_set_color(VGA_COLOR_LIGHT_BROWN, VGA_COLOR_RED);
    vga_puts_at(4, 4, "A fatal error has occurred. Enter the debug shell below.");
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_puts_at(4, 6, "Error:");
    vga_puts_at(11, 6, message);
    vga_draw_box(3, 3, VGA_WIDTH - 6, 7, panic_color);
    vga_set_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_RED);
    vga_puts_at(4, 9, "Debug commands: mem, regs, stack, vga, perf, halt");
    vga_set_color(prev & 0x0F, prev >> 4);
    vga_row = 12;
    vga_column = 0;
}

void vga_prompt() {
    uint8_t prev = vga_color;
    vga_set_color(VGA_COLOR_LIGHT_CYAN, VGA_COLOR_BLACK);
    vga_puts("[XENOos] ");
    vga_set_color(prev & 0x0F, prev >> 4);
    vga_puts("> ");
}

void vga_scroll() {
    for (size_t y = 2; y < VGA_HEIGHT - 1; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[(y - 1) * VGA_WIDTH + x] = vga_buffer[y * VGA_WIDTH + x];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_buffer[(VGA_HEIGHT - 2) * VGA_WIDTH + x] = vga_entry(' ', vga_color);
    }
    if (vga_row > 1) {
        vga_row--;
    }
}

void vga_putc(char c) {
    if (c == '\n') {
        vga_column = 0;
        if (++vga_row == VGA_HEIGHT - 1) {
            vga_scroll();
        }
        return;
    }

    if (c == 8) {  // Backspace
        if (vga_column > 0) {
            vga_column--;
        } else if (vga_row > 1) {
            vga_row--;
            vga_column = VGA_WIDTH - 1;
        }
        vga_buffer[vga_row * VGA_WIDTH + vga_column] = vga_entry(' ', vga_color);
        return;
    }

    vga_buffer[vga_row * VGA_WIDTH + vga_column] = vga_entry(c, vga_color);
    if (++vga_column == VGA_WIDTH) {
        vga_column = 0;
        if (++vga_row == VGA_HEIGHT - 1) {
            vga_scroll();
        }
    }
}

void vga_puts(const char* str) {
    for (size_t i = 0; str[i] != '\0'; i++) {
        vga_putc(str[i]);
    }
}

void vga_clear() {
    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            vga_buffer[y * VGA_WIDTH + x] = vga_entry(' ', vga_color);
        }
    }
    vga_row = 0;
    vga_column = 0;
}

void vga_draw_title(const char* title) {
    uint8_t prev = vga_color;
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_putc_at(' ', x, 0);
    }
    vga_puts_at(2, 0, title);
    vga_puts_at(VGA_WIDTH - 18, 0, "XENOos Console");
    vga_set_color(prev & 0x0F, prev >> 4);
}

void vga_draw_status(const char* status) {
    uint8_t prev = vga_color;
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLUE);
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        vga_putc_at(' ', x, VGA_HEIGHT - 1);
    }
    vga_puts_at(2, VGA_HEIGHT - 1, status);
    vga_set_color(prev & 0x0F, prev >> 4);
}

void vga_draw_ui(const char* title, const char* status) {
    vga_clear();
    vga_draw_title(title);
    vga_draw_status(status);
    vga_row = 1;
    vga_column = 0;
}

#endif // VGA_H