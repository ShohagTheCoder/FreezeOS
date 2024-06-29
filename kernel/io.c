#include "includes/io.h"

// Recive a byte to an I/O port
uint8_t inb(uint16_t port)
{
    uint8_t ret;
    asm volatile("inb %1, %0"
                 : "=a"(ret) // Return value will be store in ret variable
                 : "Nd"(port));

    return ret;
}

// Send a byte to an I/O port
void outb(uint16_t port, uint8_t val)
{
    asm volatile("outb %0, %1"
                 : // No output
                 : "a"(val), "Nd"(port));
}
