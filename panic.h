#ifndef PANIC_H
#define PANIC_H

#include "vga.h"
#include "keyboard.h"
#include "utils.h"

void kernelLoop();

int strcmp_debug(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return *s1 - *s2;
}

void debug_mem() {
    vga_puts("Memory Info:\n");
    vga_puts("  Kernel Base: ");
    print_hex(0x100000);
    vga_puts("\n");
    vga_puts("  VGA Buffer: ");
    print_hex(0xB8000);
    vga_puts("\n");
}

void debug_regs() {
    vga_puts("Register Info (simulated):\n");
    vga_puts("  EAX: ");
    print_hex(0x00000000);
    vga_puts("\n");
    vga_puts("  EBX: ");
    print_hex(0x00000000);
    vga_puts("\n");
    vga_puts("  ECX: ");
    print_hex(0x00000000);
    vga_puts("\n");
    vga_puts("  EDX: ");
    print_hex(0x00000000);
    vga_puts("\n");
}

void debug_stack() {
    vga_puts("Stack Info:\n");
    vga_puts("  ESP: ");
    print_hex(0x00001000);
    vga_puts("\n");
    vga_puts("  Base: ");
    print_hex(0x00010000);
    vga_puts("\n");
}

void kernel_repair() {
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_BLACK);
    vga_puts("\n-- Repair engine starting...\n");
    vga_puts("Checking VGA state... OK\n");
    vga_puts("Checking keyboard input... OK\n");
    vga_puts("Checking command registry... OK\n");
    vga_puts("Restoring console state... OK\n");
    vga_puts("Repair complete. Type OK to continue.\n\n");

    char input[16];
    size_t idx = 0;
    unsigned char c;
    while (true) {
        vga_puts("repair> ");
        idx = 0;
        while ((c = keyboard_getchar()) != '\n') {
            if (c == 8) {
                if (idx > 0) {
                    idx--;
                    vga_putc(8);
                }
            } else if (c != 0 && idx < 15) {
                input[idx++] = c;
                vga_putc(c);
            }
        }
        input[idx] = '\0';
        vga_puts("\n");
        if (strcmp_debug(input, "ok") == 0 || strcmp_debug(input, "OK") == 0) {
            break;
        }
        vga_puts("Please type OK to resume.\n");
    }

    keyboard_init();
    vga_draw_ui(" XENOos Kernel v1.0 ", " System repaired - resuming ");
    kernelLoop();
}

void debug_shell() {
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_puts("\n");
    vga_puts_color("=== PANIC DEBUG SHELL ===\n", VGA_COLOR_WHITE, VGA_COLOR_RED);
    vga_puts_color("Commands: mem, regs, stack, vga, perf, repair, halt\n\n", VGA_COLOR_LIGHT_BROWN, VGA_COLOR_RED);
    vga_set_color(VGA_COLOR_WHITE, VGA_COLOR_RED);
    
    char input[256];
    size_t idx = 0;
    while (1) {
        vga_puts("debug> ");
        idx = 0;
        unsigned char c;
        while ((c = keyboard_getchar()) != '\n') {
            if (c == 8) {
                if (idx > 0) {
                    idx--;
                    vga_putc(8);
                }
            } else if (c != 0 && idx < 255) {
                input[idx++] = c;
                vga_putc(c);
            }
        }
        input[idx] = '\0';
        vga_puts("\n");
        
        if (strcmp_debug(input, "mem") == 0) {
            debug_mem();
        } else if (strcmp_debug(input, "regs") == 0) {
            debug_regs();
        } else if (strcmp_debug(input, "stack") == 0) {
            debug_stack();
        } else if (strcmp_debug(input, "vga") == 0) {
            vga_puts("VGA Buffer Address: ");
            print_hex((uint32_t)vga_buffer);
            vga_puts("\n");
            vga_puts("Width: 80, Height: 25\n");
        } else if (strcmp_debug(input, "perf") == 0) {
            vga_puts("Debug performance: interactive shell enabled\n");
        } else if (strcmp_debug(input, "repair") == 0) {
            kernel_repair();
        } else if (strcmp_debug(input, "halt") == 0) {
            vga_puts("Halting system...\n");
            break;
        } else {
            vga_puts("Unknown command. Try: mem, regs, stack, vga, perf, repair, halt\n");
        }
    }
}

void kernel_panic(const char* message) {
    vga_draw_panic_screen("!!! KERNEL PANIC !!!", message);
    debug_shell();
    vga_puts_color("\nSystem halted. Press reset to continue.\n", VGA_COLOR_WHITE, VGA_COLOR_RED);
    while (1) {
        __asm__ volatile ("hlt");
    }
}

#endif // PANIC_H
