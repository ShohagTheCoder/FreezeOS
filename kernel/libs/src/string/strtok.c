#include <stddef.h>
#include <stdio.h>
#include <string.h>

char* strtok(char* str, const char* delim)
{
    // puts("STRTOK STR : ");
    // puts(delim);
    // puts(" | ");
    static char* last;
    if (str == NULL)
    {
        str = last;
    }

    if (str == NULL)
    {
        return NULL;
    }

    char* token = str;
    str = strpbrk(str, delim);

    if (str)
    {
        *str = '\0';
        last = str + 1;
    }
    else
    {
        last = NULL;
    }

    return token;
}