#include "multiboot.h"
#include "vga.h"
#include "commandlist.h"
#include "help.h"
#include "follow.h"
#include "commonkernel.h"
#include "keylist.h"
#include "serial-driver.h"
#include "keyboard.h"
#include "version.h"
#include "panic.h"
#include "utils.h"
#include "system.h"
#include "commands.h"

extern "C" {

__attribute__((section(".multiboot")))
const unsigned int multiboot_header[] = {
    MULTIBOOT_HEADER_MAGIC,
    MULTIBOOT_HEADER_FLAGS,
    MULTIBOOT_HEADER_CHECKSUM
};

void kernel_main() {
    vga_clear();
    vga_puts("Kernel started\n");
    kernelInit();
    kernelLoop();
}

}