#ifndef IO_H
#define IO_H

#include "types.h"

uint8_t inb(uint16_t);
void outb(uint16_t port, uint8_t val);

#endif