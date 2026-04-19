#ifndef COMMONKERNEL_H
#define COMMONKERNEL_H

#include "common.h"
#include "vga.h"
#include "keyboard.h"
#include "commands.h"

void add_history(const char* text) {
    if (text == 0 || text[0] == '\0') {
        return;
    }
    if (history_count < 10) {
        strcpy(command_history[history_count], text);
        history_count++;
        return;
    }
    for (size_t i = 1; i < 10; i++) {
        strcpy(command_history[i - 1], command_history[i]);
    }
    strcpy(command_history[9], text);
}

void update_status_bar() {
    char status[80];
    size_t len = 0;
    const char* prefix = "Uptime: ";
    for (size_t i = 0; prefix[i]; i++) {
        status[len++] = prefix[i];
    }
    char ticks[12];
    itoa(kernel_ticks, ticks, 10);
    for (size_t i = 0; ticks[i]; i++) {
        status[len++] = ticks[i];
    }
    const char* middle = " ticks | task: ";
    for (size_t i = 0; middle[i]; i++) {
        status[len++] = middle[i];
    }
    const char* taskName = kernel_tasks[current_task].name;
    for (size_t i = 0; taskName[i]; i++) {
        status[len++] = taskName[i];
    }
    status[len] = '\0';
    vga_draw_status(status);
}

void kernel_shutdown() {
    vga_puts("System shutdown...\n");
    __asm__ volatile ("hlt");
}

void kernelInit() {
    vga_clear();
    if (vga_buffer == 0) {
        kernel_panic("VGA initialization failed");
    }
    fs_init();
    scheduler_init();
    vga_draw_ui(" XENOos Kernel v1.0 ", " Type 'help' for commands ");
    vga_puts("Welcome to XENOos!\n");
    vga_puts("Use 'help' to see available commands.\n\n");
    keyboard_init();
    update_status_bar();
}

void kernelLoop() {
    vga_prompt();
    char input[256];
    size_t idx = 0;
    while (true) {
        unsigned char c = keyboard_getchar();
        if (c == '\n') {
            input[idx] = '\0';
            vga_puts("\n");
            if (idx > 0) {
                if (strcmp(input, "!!") == 0 && history_count > 0) {
                    strcpy(input, command_history[history_count - 1]);
                    idx = strlen(input);
                    vga_puts(input);
                    vga_puts("\n");
                }
                add_history(input);
                char* args = 0;
                for (size_t i = 0; i < idx; i++) {
                    if (input[i] == ' ') {
                        input[i] = '\0';
                        args = &input[i + 1];
                        while (*args == ' ') args++;
                        if (*args == '\0') {
                            args = 0;
                        }
                        break;
                    }
                }
                const KernelCommand* cmd = find_command(input);
                if (cmd != 0) {
                    cmd->handler(args);
                } else {
                    vga_puts("Unknown command: ");
                    vga_puts(input);
                    vga_puts("\n");
                }
            }
            update_status_bar();
            vga_prompt();
            idx = 0;
        } else if (c == 8) {
            if (idx > 0) {
                idx--;
                vga_putc(8);
            }
        } else if (c != 0) {
            if (idx < 255) {
                input[idx++] = c;
                vga_putc(c);
                kernel_ticks++;
            }
        }
    }
}

#endif // COMMONKERNEL_H