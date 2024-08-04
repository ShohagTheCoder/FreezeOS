#include "../../../includes/console.h"

// Print a string into console
void puts(const char* s)
{
    while (*s)
    {
        putchar(*s++);
    }
}