#ifndef CONSOLE_H
#define CONSOLE_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

static void outb(uint16_t port, uint8_t val);
void print_string(const char *str);
void clear_screen();
void putchar(char c);
void move_cursor();

#endif
