#include <stdint.h>
#include "../../../includes/console.h"

// Print a string into console
void puts(const char* s)
{
    while (*s)
    {
        putchar(*s++);
    }
}

void d(char* title, char* data)
{
    puts(title);
    puts(" : ");
    puts(data);
    hr();
}