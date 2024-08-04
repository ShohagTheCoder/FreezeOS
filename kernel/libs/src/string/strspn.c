#include <stddef.h>

size_t strspn(const char* str, char* accept)
{
    const char* p;
    const char* a;
    size_t count = 0;

    for (p = str; *p != '\0'; p++)
    {
        for (a = accept; *a != '\0'; a++)
        {
            if (*a == *p)
            {
                count++;
                break;
            }
        }
    }

    return count;
}