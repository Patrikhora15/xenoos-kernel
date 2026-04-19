#ifndef SYSTEM_H
#define SYSTEM_H

#include "common.h"
#include "vga.h"
#include "utils.h"

static uint32_t kernel_ticks = 0;
static const size_t FS_MAX_ENTRIES = 16;
static const size_t MAX_TASKS = 4;

struct FsEntry {
    const char* path;
    const char* content;
    bool isDir;
};

static FsEntry fs_entries[FS_MAX_ENTRIES];
static size_t fs_count = 0;

struct KernelTask {
    const char* name;
    bool active;
    uint32_t runTicks;
};

static KernelTask kernel_tasks[MAX_TASKS];
static size_t task_count = 0;
static size_t current_task = 0;
static char command_history[10][256];
static size_t history_count = 0;

void fs_add(const char* path, const char* content, bool isDir) {
    if (fs_count < FS_MAX_ENTRIES) {
        fs_entries[fs_count].path = path;
        fs_entries[fs_count].content = content;
        fs_entries[fs_count].isDir = isDir;
        fs_count++;
    }
}

const FsEntry* fs_find(const char* path) {
    for (size_t i = 0; i < fs_count; i++) {
        if (strcmp(fs_entries[i].path, path) == 0) {
            return &fs_entries[i];
        }
    }
    return 0;
}

bool is_child_entry(const char* path, const char* dir) {
    if (strcmp(path, dir) == 0) {
        return false;
    }
    if (strcmp(dir, "/") == 0) {
        size_t slashCount = 0;
        for (size_t i = 0; path[i]; i++) {
            if (path[i] == '/') {
                slashCount++;
            }
        }
        return slashCount == 1;
    }
    size_t dirLen = strlen(dir);
    if (strncmp(path, dir, dirLen) != 0 || path[dirLen] != '/') {
        return false;
    }
    const char* rest = path + dirLen + 1;
    for (size_t i = 0; rest[i]; i++) {
        if (rest[i] == '/') {
            return false;
        }
    }
    return true;
}

void fs_init() {
    fs_count = 0;
    fs_add("/", 0, true);
    fs_add("/boot", 0, true);
    fs_add("/sys", 0, true);
    fs_add("/dev", 0, true);
    fs_add("/boot/kernel.bin", "XENOos kernel binary placeholder\n", false);
    fs_add("/sys/info.txt", "XENOos system information file\n", false);
}

void fs_list(const char* dir) {
    const FsEntry* directory = fs_find(dir);
    if (!directory || !directory->isDir) {
        vga_puts("Directory not found: ");
        vga_puts(dir);
        vga_puts("\n");
        return;
    }
    vga_puts("Directory listing for ");
    vga_puts(dir);
    vga_puts(":\n");
    for (size_t i = 0; i < fs_count; i++) {
        if (is_child_entry(fs_entries[i].path, dir)) {
            const char* path = fs_entries[i].path;
            const char* name = path;
            for (size_t j = strlen(path); j > 0; j--) {
                if (path[j - 1] == '/') {
                    name = path + j;
                    break;
                }
            }
            if (fs_entries[i].isDir) {
                vga_puts("  [DIR] ");
            } else {
                vga_puts("  [FILE] ");
            }
            vga_puts(name);
            vga_puts("\n");
        }
    }
}

void fs_cat(const char* path) {
    if (!path) {
        vga_puts("Usage: cat <path>\n");
        return;
    }
    const FsEntry* file = fs_find(path);
    if (!file) {
        vga_puts("File not found: ");
        vga_puts(path);
        vga_puts("\n");
        return;
    }
    if (file->isDir) {
        vga_puts("Cannot cat directory: ");
        vga_puts(path);
        vga_puts("\n");
        return;
    }
    vga_puts(file->content);
}

void fs_make_dir(const char* path) {
    if (!path) {
        vga_puts("Usage: mkdir <path>\n");
        return;
    }
    if (fs_find(path)) {
        vga_puts("Directory already exists: ");
        vga_puts(path);
        vga_puts("\n");
        return;
    }
    fs_add(path, 0, true);
    vga_puts("Directory created: ");
    vga_puts(path);
    vga_puts("\n");
}

void fs_touch(const char* path) {
    if (!path) {
        vga_puts("Usage: touch <path>\n");
        return;
    }
    const FsEntry* entry = fs_find(path);
    if (entry) {
        if (entry->isDir) {
            vga_puts("Cannot touch directory: ");
            vga_puts(path);
            vga_puts("\n");
            return;
        }
        vga_puts("File already exists: ");
        vga_puts(path);
        vga_puts("\n");
        return;
    }
    fs_add(path, "", false);
    vga_puts("File created: ");
    vga_puts(path);
    vga_puts("\n");
}

void scheduler_init() {
    task_count = 3;
    current_task = 0;
    kernel_tasks[0] = {"Idle", true, 0};
    kernel_tasks[1] = {"Renderer", true, 0};
    kernel_tasks[2] = {"Logger", true, 0};
}

void scheduler_next() {
    if (task_count == 0) {
        return;
    }
    current_task = (current_task + 1) % task_count;
    kernel_tasks[current_task].runTicks++;
    vga_puts("Switched to task: ");
    vga_puts(kernel_tasks[current_task].name);
    vga_puts("\n");
}

void cmd_clear() {
    vga_clear();
}

void cmd_echo(const char* args) {
    if (args) {
        vga_puts(args);
    }
    vga_puts("\n");
}

void cmd_cpuinfo() {
    vga_puts("CPU Information:\n");
    vga_puts("  Arch: x86 (32-bit)\n");
    vga_puts("  Mode: Protected Mode\n");
    vga_puts("  Bootloader: GRUB Multiboot\n");
}

void cmd_meminfo() {
    vga_puts("Memory Information:\n");
    vga_puts("  Kernel Base: ");
    print_hex(0x100000);
    vga_puts("\n");
    vga_puts("  VGA Buffer: ");
    print_hex(0xB8000);
    vga_puts("\n");
    vga_puts("  Available: ~3GB\n");
}

void cmd_uptime() {
    vga_puts("Kernel Uptime: ");
    print_dec(kernel_ticks);
    vga_puts(" ticks\n");
}

void cmd_whoami() {
    vga_puts("XENOos Kernel - v1.0\n");
}

void cmd_date() {
    vga_puts("Date: ");
    vga_puts(__DATE__);
    vga_puts("\n");
}

static inline void outb(uint16_t port, uint8_t value) {
    __asm__ volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

void cmd_status() {
    vga_puts("Kernel Status:\n");
    vga_puts("  Uptime: ");
    print_dec(kernel_ticks);
    vga_puts(" ticks\n");
    vga_puts("  Current task: ");
    vga_puts(kernel_tasks[current_task].name);
    vga_puts("\n");
    vga_puts("  Task run ticks: ");
    print_dec(kernel_tasks[current_task].runTicks);
    vga_puts("\n");
}

void cmd_perf() {
    vga_puts("Performance Metrics:\n");
    vga_puts("  Uptime ticks: ");
    print_dec(kernel_ticks);
    vga_puts("\n");
    vga_puts("  Task count: ");
    print_dec(task_count);
    vga_puts("\n");
    vga_puts("  Current task: ");
    vga_puts(kernel_tasks[current_task].name);
    vga_puts("\n");
}

void cmd_history() {
    vga_puts("Command History:\n");
    for (size_t i = 0; i < history_count; i++) {
        vga_puts("  ");
        vga_puts(command_history[i]);
        vga_puts("\n");
    }
}

void cmd_shutdown() {
    vga_puts("Shutting down...\n");
    __asm__ volatile ("hlt");
}

void cmd_reboot() {
    vga_puts("Rebooting...\n");
    outb(0x64, 0xFE);
    while (1) {
        __asm__ volatile ("hlt");
    }
}

void cmd_ps() {
    vga_puts("Running Processes:\n");
    for (size_t i = 0; i < task_count; i++) {
        vga_puts("  PID ");
        print_dec(i);
        vga_puts(" - ");
        vga_puts(kernel_tasks[i].name);
        if (i == current_task) {
            vga_puts(" [active]");
        }
        vga_puts("\n");
    }
}

void cmd_ls(const char* args) {
    if (args && args[0] != '\0') {
        fs_list(args);
    } else {
        fs_list("/");
    }
}

void cmd_cat(const char* args) {
    fs_cat(args);
}

void cmd_mkdir(const char* args) {
    fs_make_dir(args);
}

void cmd_touch(const char* args) {
    fs_touch(args);
}

void cmd_schedule() {
    scheduler_next();
}

void cmd_yield() {
    scheduler_next();
}

#endif // SYSTEM_H
