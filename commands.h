#ifndef COMMANDS_H
#define COMMANDS_H

#include "common.h"
#include "vga.h"
#include "utils.h"
#include "help.h"
#include "follow.h"
#include "version.h"
#include "panic.h"
#include "system.h"

typedef void (*cmd_handler)(const char*);

struct KernelCommand {
    const char* name;
    cmd_handler handler;
    const char* description;
};

void cmd_help_handler(const char* args) {
    (void)args;
    showHelp();
}

void cmd_version_handler(const char* args) {
    (void)args;
    showVersion();
}

void cmd_exit_handler(const char* args) {
    (void)args;
    vga_puts("Exiting...\n");
    __asm__ volatile ("hlt");
}

void cmd_shutdown_handler(const char* args) {
    (void)args;
    cmd_shutdown();
}

void cmd_reboot_handler(const char* args) {
    (void)args;
    cmd_reboot();
}

void cmd_status_handler(const char* args) {
    (void)args;
    cmd_status();
}

void cmd_panic_handler(const char* args) {
    (void)args;
    kernel_panic("User triggered kernel panic");
}

void cmd_clear_handler(const char* args) {
    (void)args;
    cmd_clear();
}

void cmd_echo_handler(const char* args) {
    cmd_echo(args);
}

void cmd_cpuinfo_handler(const char* args) {
    (void)args;
    cmd_cpuinfo();
}

void cmd_meminfo_handler(const char* args) {
    (void)args;
    cmd_meminfo();
}

void cmd_uptime_handler(const char* args) {
    (void)args;
    cmd_uptime();
}

void cmd_whoami_handler(const char* args) {
    (void)args;
    cmd_whoami();
}

void cmd_perf_handler(const char* args) {
    (void)args;
    cmd_perf();
}

void cmd_history_handler(const char* args) {
    (void)args;
    cmd_history();
}

void cmd_date_handler(const char* args) {
    (void)args;
    cmd_date();
}

void cmd_ps_handler(const char* args) {
    (void)args;
    cmd_ps();
}

void cmd_ls_handler(const char* args) {
    cmd_ls(args);
}

void cmd_cat_handler(const char* args) {
    cmd_cat(args);
}

void cmd_mkdir_handler(const char* args) {
    cmd_mkdir(args);
}

void cmd_touch_handler(const char* args) {
    cmd_touch(args);
}

void cmd_schedule_handler(const char* args) {
    (void)args;
    cmd_schedule();
}

void cmd_yield_handler(const char* args) {
    (void)args;
    cmd_yield();
}

static const KernelCommand commands[] = {
    {"help", cmd_help_handler, "show this help"},
    {"version", cmd_version_handler, "show kernel version"},
    {"kernelp", cmd_panic_handler, "trigger kernel panic"},
    {"exit", cmd_exit_handler, "exit the kernel"},
    {"clear", cmd_clear_handler, "clear screen"},
    {"echo", cmd_echo_handler, "echo text"},
    {"cpuinfo", cmd_cpuinfo_handler, "show CPU info"},
    {"meminfo", cmd_meminfo_handler, "show memory info"},
    {"uptime", cmd_uptime_handler, "show kernel uptime"},
    {"status", cmd_status_handler, "show kernel status"},
    {"perf", cmd_perf_handler, "show performance metrics"},
    {"history", cmd_history_handler, "show command history"},
    {"whoami", cmd_whoami_handler, "show system info"},
    {"date", cmd_date_handler, "show date"},
    {"ps", cmd_ps_handler, "list processes"},
    {"ls", cmd_ls_handler, "list filesystem"},
    {"cat", cmd_cat_handler, "show file contents"},
    {"mkdir", cmd_mkdir_handler, "create directory"},
    {"touch", cmd_touch_handler, "create file"},
    {"schedule", cmd_schedule_handler, "switch scheduler task"},
    {"yield", cmd_yield_handler, "yield to next task"},
    {"shutdown", cmd_shutdown_handler, "shutdown the system"},
    {"reboot", cmd_reboot_handler, "reboot the system"},
    {0, 0, 0}
};

const KernelCommand* find_command(const char* name) {
    for (size_t i = 0; commands[i].name != 0; i++) {
        if (strcmp(name, commands[i].name) == 0) {
            return &commands[i];
        }
    }
    return 0;
}

#endif // COMMANDS_H
