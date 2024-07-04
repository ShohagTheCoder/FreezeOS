#include "../includes/drivers/keyboard.h"
#include "../includes/drivers/console.h"
#include "../includes/lib/string.h"

#define CODE_SEGMENT_SELECTOR 0x80

struct idt_entry idt[256];
struct idt_ptr idtp;

void keyboard_handler();

// Set idt entry
void set_idt_entry(uint32_t n, size_t handler)
{
    idt[n].offset_low = handler & 0xFFFF;
    idt[n].selector = CODE_SEGMENT_SELECTOR;
    idt[n].zero = 0;
    idt[n].type_attr = 0x8E; // Interrupt gate
    idt[n].offset_high = (handler >> 16) & 0xFFFF;
}

// Initialize the IDT
void init_idt()
{
    print_str("init_idt\n");
    idtp.limit = (sizeof(struct idt_entry) * 256) - 1;
    idtp.base = (uint32_t)&idt;

    for (int i = 0; i < 256; i++)
    {
        // set_idt_entry(i, (uint32_t)0);
        set_idt_entry(i, (uint32_t)keyboard_handler);
    }

    // Load the idt
    asm volatile("lidt %0" : : "m"(idtp));
}

void keyboard_handler()
{
    putchar('c');
    return;
}