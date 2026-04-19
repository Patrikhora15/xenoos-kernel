#ifndef FOLLOW_H
#define FOLLOW_H

#include "common.h"
#include "vga.h"

void followCommand(const char* cmd) {
    vga_puts("Following command: ");
    vga_puts(cmd);
    vga_puts("\n");
    // implement logic
}

#endif // FOLLOW_H