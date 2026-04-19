#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "common.h"

#define KEY_BACKSPACE 0x0E
#define KEY_ENTER 0x1C

static const char scancode_to_ascii[] = {
    0, 0, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', 0, 0,
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0, '*', 0, ' ', 0
};

unsigned char keyboard_getchar() {
    unsigned char scancode;
    do {
        __asm__ volatile ("inb $0x64, %%al" : "=a"(scancode));
    } while (!(scancode & 1));
    __asm__ volatile ("inb $0x60, %%al" : "=a"(scancode));
    
    if (scancode == KEY_BACKSPACE) {
        return 8;  // Return backspace character
    }
    
    if (scancode < sizeof(scancode_to_ascii) / sizeof(scancode_to_ascii[0])) {
        return scancode_to_ascii[scancode];
    }
    return 0;
}

void keyboard_init() {
    // Enable keyboard
}

#endif // KEYBOARD_H