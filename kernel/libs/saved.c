char* fz_strtok(char* str, const char* delimiter)
{
    static char* save_pointer;

    if (str == NULL)
    {
        str = save_pointer;
    }

    if (str == '\0')
    {
        return NULL;
    }

    // Skip leading delimiters

    str += fz_strcspn(str, delimiter);

    if (*str == '\0')
    {
        return NULL;
    }

    /* Find the end of the token */
    char* token = str;
    str += fz_strcspn(str, delimiter);

    if (*str != '\0')
    {
        *str++ = '\0';
    }

    save_pointer = str;
    return token;
}

size_t fz_strcspn(char* str, const char* delimiter)
{
    size_t count = 0;

    while (*str != '\0')
    {
        if (fz_strchr(delimiter, *str) != NULL)
        {
            break;
        }

        count++;
        str++;
    }
}

char* fz_strchr(const char* str, int c)
{
    while (*str != '\0')
    {
        if (*str == (char)c)
        {
            return (char*)c;
        }

        str++;
    }

    return NULL;
}