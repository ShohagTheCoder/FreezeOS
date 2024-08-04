#include <stddef.h>

char* strpbrk(const char* str, const char* accept)
{
    while (*str)
    {
        const char* a = accept;
        while (*a)
        {
            if (*a == *str)
            {
                return (char*)str;
            }
            a++;
        }
        str++;
    }
    return NULL;
}