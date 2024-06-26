#ifndef CONSOLE_H
#define CONSOLE_H

typedef unsigned char uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int uint32_t;
typedef unsigned long long uint64_t;

void move_cursor();
void clear_screen();
void putchar(char c);
void prints(const char *str);

#endif
