#ifndef INTERRUPT_H
#define INTERRUPT_H

#include "common.h"

void handleInterrupt(int id) {
    std::cout << "Handling interrupt: " << id << std::endl;
}

#endif // INTERRUPT_H