#include <stddef.h>
#include "../../../includes/console.h"

// Print characters n number of times
void putns(const char* str, size_t n)
{
    while (n--)
    {
        putchar(*str++);
    }
}