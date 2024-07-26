#ifndef IO_H
#define IO_H

#include "types.h"

uint8_t inb(uint16_t);
void outb(uint16_t port, uint8_t val);
uint16_t inw(uint16_t);
void outw(uint16_t port, uint16_t val);

#endif