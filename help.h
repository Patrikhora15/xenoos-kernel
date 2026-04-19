#ifndef HELP_H
#define HELP_H

#include "common.h"
#include "vga.h"
#include "keyboard.h"

static const char* help_lines[] = {
    "=== XENOos Help ===",
    "Available commands:",
    "  help     - show this help",
    "  version  - show kernel version",
    "  clear    - clear screen",
    "  echo     - echo text",
    "  cpuinfo  - show CPU information",
    "  meminfo  - show memory information",
    "  uptime   - show kernel uptime",
    "  status   - show kernel status",
    "  perf     - show performance metrics",
    "  history  - show command history",
    "  whoami   - show system information",
    "  date     - show current date",
    "  ls       - list filesystem",
    "  cat      - show file contents",
    "  mkdir    - create directory",
    "  touch    - create file",
    "  schedule - switch scheduler task",
    "  yield    - yield to next task",
    "  reboot   - reboot the system",
    "  shutdown - shutdown the system",
    "  kernelp  - trigger kernel panic",
    "  exit     - exit the kernel",
    0
};

void showHelp() {
    const size_t lines_per_page = 10;
    size_t total_lines = 0;
    while (help_lines[total_lines]) total_lines++;
    
    size_t current_line = 0;
    vga_clear();
    while (current_line < total_lines) {
        // Print up to lines_per_page
        size_t end_line = current_line + lines_per_page;
        if (end_line > total_lines) end_line = total_lines;
        
        for (size_t i = current_line; i < end_line; i++) {
            vga_puts(help_lines[i]);
            vga_puts("\n");
        }
        
        current_line = end_line;
        
        if (current_line < total_lines) {
            vga_puts("\nPress Enter to continue, 'q' to quit...");
            unsigned char c;
            do {
                c = keyboard_getchar();
            } while (c != '\n' && c != 'q');
            
            if (c == 'q') {
                vga_puts("\n");
                break;
            }
            vga_puts("\n");
        }
    }
    vga_draw_ui(" XENOos Kernel v1.0 ", " Type 'help' for commands ");
}

#endif // HELP_H