char* strncpy(char* dest, const char* src, int n)
{
    int i;
    for (i = 0; i < n && src[i] != '\0'; i++)
    {
        dest[i] = src[i];
    }

    // Fill remaing spaces with null-terminators
    for (; i < n; i++)
    {
        dest[i] = '\0';
    }

    return dest;
}