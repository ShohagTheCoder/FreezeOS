#ifndef KEYBOARD_H
#define KEYBOARD_H

#include "../../../includes/types/types.h"

struct idt_entry
{
    uint16_t offset_low;
    uint16_t selector;
    uint8_t zero;
    uint8_t type_attr;
    uint16_t offset_high;
} __attribute__((packed));

struct idt_ptr
{
    uint16_t limit;
    size_t base;
} __attribute__((packed));

void init_idt();

#endif