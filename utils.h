#ifndef UTILS_H
#define UTILS_H

#include "common.h"
#include "vga.h"

size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len]) len++;
    return len;
}

void itoa(int num, char* str, int base) {
    if (num == 0) {
        str[0] = '0';
        str[1] = '\0';
        return;
    }
    
    char digits[] = "0123456789ABCDEF";
    char temp[32];
    int i = 0;
    int negative = (num < 0 && base == 10);
    
    if (negative) num = -num;
    
    while (num > 0) {
        temp[i++] = digits[num % base];
        num /= base;
    }
    
    int j = 0;
    if (negative) str[j++] = '-';
    
    while (i > 0) {
        str[j++] = temp[--i];
    }
    str[j] = '\0';
}

void strcpy(char* dest, const char* src) {
    while ((*dest++ = *src++) != '\0') {}
}

int strcmp(const char* s1, const char* s2) {
    while (*s1 && *s2 && *s1 == *s2) {
        s1++;
        s2++;
    }
    return (unsigned char)*s1 - (unsigned char)*s2;
}

int strncmp(const char* s1, const char* s2, size_t n) {
    for (size_t i = 0; i < n; i++) {
        if (s1[i] != s2[i] || s1[i] == '\0' || s2[i] == '\0') {
            return (unsigned char)s1[i] - (unsigned char)s2[i];
        }
    }
    return 0;
}

void print_hex(uint32_t value) {
    char buffer[12];
    vga_puts("0x");
    itoa(value, buffer, 16);
    vga_puts(buffer);
}

void print_dec(int value) {
    char buffer[12];
    itoa(value, buffer, 10);
    vga_puts(buffer);
}

#endif // UTILS_H
