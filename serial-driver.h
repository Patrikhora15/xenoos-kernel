#ifndef SERIAL_DRIVER_H
#define SERIAL_DRIVER_H

#include "common.h"
#include "vga.h"

class SerialDriver {
public:
    void init() {
        vga_puts("Serial driver initialized\n");
    }
    void send(byte data) {
        vga_puts("Sending: ");
        vga_putc((char)(data + '0')); // dummy
        vga_puts("\n");
    }
    byte receive() {
        return 0; // dummy
    }
};

#endif // SERIAL_DRIVER_H