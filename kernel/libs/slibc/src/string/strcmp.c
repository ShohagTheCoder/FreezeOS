#include <stddef.h>

// Compare a string
int strcmp(const char* a, const char* b)
{
    while (*a && (*a == *b))
    {
        a++;
        b++;
    }

    return *(unsigned char*)a - *(unsigned char*)b;
}