#!/bin/bash
# Build script for the OS kernel

# Compile
g++ -c -m32 -ffreestanding -nostdlib main.cpp -o main.o

# Link
ld -m elf_i386 -T linker.ld main.o -o kernel.elf

echo "Build complete. Run with: qemu-system-x86_64 -kernel kernel.elf"