#ifndef MEMORY_H
#define MEMORY_H

#include "common.h"
#include <cstdlib>

void* allocateMemory(size_t size) {
    return malloc(size);
}

void freeMemory(void* ptr) {
    free(ptr);
}

#endif // MEMORY_H