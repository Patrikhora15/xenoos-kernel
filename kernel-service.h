#ifndef KERNEL_SERVICE_H
#define KERNEL_SERVICE_H

#include "common.h"
#include "vga.h"

class KernelService {
public:
    void startService() {
        vga_puts("Kernel service started\n");
    }
    void stopService() {
        vga_puts("Kernel service stopped\n");
    }
};

#endif // KERNEL_SERVICE_H