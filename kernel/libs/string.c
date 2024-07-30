#include "../includes/string.h"
#include "../includes/console.h"

static char* ptr;

/**
 * @brief Counts the number of character in the string.
 *
 * This function take a string pointer and count ther length by looping the
 * string.
 *
 * @param str The pointer which locate the string starting position.
 * @return The lenght of the string a
 */
int strlen(char* str)
{
    int len = 0;
    while (str[len] != '\0')
    {
        if (len > 100)
            break;
        len++;
    }

    return len;
}

// Append new charater to string
void str_append(char str[], char n)
{
    int len = strlen(str);
    str[len] = n;
    str[len + 1] = '\0';
}

int str_remove(char str[])
{
    int len = strlen(str);

    if (len > 0)
    {
        str[len - 1] = '\0';
        return 1;
    }
    else
    {
        return 0;
    }
}

// Function to convert an integer to its ASCII representation
void int_to_ascii(int n, char str[])
{
    // Counts of the digits
    int counts = count_digits(n);

    // Handle negative numbers
    if (n < 0)
    {
        n = -n;
        str[0] = '-';
        counts++;
    }

    // Set null terminate to the end of string
    str[counts] = '\0';

    // Generate digits
    do
    {
        str[--counts] = n % 10 + '0';  // Convert each digit to ASCII
        n /= 10;
    } while (n > 0);
}

int str_cmp_in(char* str1, char* str2, int count)
{
    while (count && *str1 && (*str1 == *str2))
    {
        count--;
        str1++;
        str2++;
    }
    return count == 0 ? 1 : 0;
}

int str_cmp(char* str1, char* str2)
{
    int count = strlen(str1);

    while (count && *str1 && (*str1 == *str2))
    {
        count--;
        str1++;
        str2++;
    }
    return count == 0 ? 1 : 0;
}

void reset_str(char* str)
{
    int len = strlen(str);
    for (size_t i = 0; i < len; ++i)
    {
        str[i] = '\0';  // Set each character to null ('\0')
    }
}

/**
 * Print string function
 */
void print_str(char* str)
{
    while (*str)
    {
        putchar(*str++);
    }
}

void print_str_in(char* str, int count)
{
    while (count--)
    {
        putchar(*str++);
    }
}

// Example implementation of print_int assuming screen output functions exist
void print_int(int n)
{
    // Create a empty array with length equal to the counts of the number to
    // hold the returen string
    char str[count_digits(n)];
    int_to_ascii(n, str);

    // Assuming you have a function to print strings to the screen
    print_str(str);  // Replace with your screen output function
}

void reverse_str(char* str)
{
    int len = strlen(str);

    // Swap charaters from start to end
    for (int i = 0; i < len / 2; i++)
    {
        char temp = str[i];
        str[i] = str[len - i - 1];
        str[len - i - 1] = temp;
    }
}

int count_digits(int n)
{
    int count = 0;

    // Handle negative ns
    if (n < 0)
    {
        n = -n;
    }

    // Handle the special case of 0
    if (n == 0)
    {
        return 1;
    }

    // Count the n of digits
    while (n != 0)
    {
        n /= 10;
        count++;
    }

    return count;
}

void fz_strcpy(char* src, char* dest)
{
    while (*src)
    {
        *dest++ = *src++;
    }
}

void fz_strncpy(char* src, char* dest, int count)
{
    if (dest == NULL)
    {
        dest = ptr;
    }

    while (count > 0)
    {
        *dest++ = *src++;
        count--;
    }

    ptr = dest;
}

void fz_strcpy_max(char* src, char* dest, int max)
{
    while (*src && max--)
    {
        *dest++ = *src++;
    }
}

void fz_fill_spaces(char* str, int end)
{
    int start = strlen(str);
    for (int i = start; i < end; i++)
    {
        str[i] = ' ';
    }

    str[end] = '\0';
}

void fz_substr(char str[], char* substr, int start, int count)
{
    count += start;
    int index = start;
    for (int i = start; i < count; i++)
    {
        if (str[i] == '\0')
        {
            substr[i - index] = ' ';
        }
        else
        {
            substr[i - index] = str[i];
        }
    }

    substr[count - index] = '\0';
}

void fz_to_uppercase(char* str)
{
    while (*str)
    {
        if (*str >= 'a' && *str <= 'z')
        {
            *str -= 32;
        }
        str++;
    }
}

void fz_to_lowercase(char* str)
{
    while (*str)
    {
        if (*str >= 'A' && *str <= 'Z')
        {
            *str += 32;
        }
        str++;
    }
}

char* fz_strchr(const char* str, int c)
{
    while (*str)
    {
        if (*str == (char)c)
        {
            return (char*)str;
        }
        str++;
    }
    return NULL;
}

char* fz_strpbrk(const char* str, const char* accept)
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

size_t fz_strspn(const char* str, char* accept)
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

char* fz_strtok(char* str, const char* delim)
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

    // str += fz_strspn(str, delim);  // Skip leading dilimiters

    // if (*str == '\0')
    // {
    //     return NULL;
    // }

    char* token = str;
    str = fz_strpbrk(str, delim);

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

void fz_strsplit(char* str, char** pointers, char* delim)
{
    char* token = fz_strtok(str, delim);
    int count = 0;

    while (token != NULL)
    {
        pointers[count] = token;
        count++;
        token = fz_strtok(NULL, delim);
    }
}
