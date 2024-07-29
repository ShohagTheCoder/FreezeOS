char* my_strchr(const char* str, int c)
{
    while (*str)
    {
        if (*str == (char)c)
        {
            return (char*)str;
        }
        str++;
    }
    return NULL;  // If character not found, return NULL
}

char* my_strncpy(char* dest, const char* src, size_t n)
{
    size_t i;
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }
    for (; i < n; i++)
    {
        dest[i] = '\0';
    }
    return dest;
}

char* my_strtok(char* str, const char* delim)
{
    static char* last;
    if (str == NULL)
    {
        str = last;
    }
    if (str == NULL)
    {
        return NULL;
    }

    str += strspn(str, delim);  // Skip leading delimiters

    if (*str == '\0')
    {
        return NULL;
    }

    char* token = str;
    str = strpbrk(token, delim);  // Find the end of the token

    if (str)
    {
        *str = '\0';  // Null-terminate the token
        last = str + 1;
    }
    else
    {
        last = NULL;
    }

    return token;
}

#include <stdlib.h>
#include <string.h>

char** my_strsplit(const char* str, const char* delim, size_t* numTokens)
{
    size_t tokens_alloc = 10;
    size_t tokens_used = 0;
    char** tokens = calloc(tokens_alloc, sizeof(char*));
    char* str_copy = strdup(str);
    char* token;

    token = strtok(str_copy, delim);
    while (token != NULL)
    {
        if (tokens_used == tokens_alloc)
        {
            tokens_alloc *= 2;
            tokens = realloc(tokens, tokens_alloc * sizeof(char*));
        }
        tokens[tokens_used++] = strdup(token);
        token = strtok(NULL, delim);
    }

    // Null-terminate the array
    tokens = realloc(tokens, (tokens_used + 1) * sizeof(char*));
    tokens[tokens_used] = NULL;

    *numTokens = tokens_used;
    free(str_copy);
    return tokens;
}

size_t my_strspn(const char* s, const char* accept)
{
    const char* p;
    const char* a;
    size_t count = 0;

    for (p = s; *p != '\0'; p++)
    {
        for (a = accept; *a != '\0'; a++)
        {
            if (*p == *a)
            {
                break;
            }
        }
        if (*a == '\0')
        {
            break;
        }
        count++;
    }

    return count;
}

char* my_strpbrk(const char* str, const char* accept)
{
    while (*str)
    {
        const char* a = accept;
        while (*a)
        {
            if (*str == *a)
            {
                return (char*)str;
            }
            a++;
        }
        str++;
    }
    return NULL;
}
