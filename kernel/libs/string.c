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
size_t strlen(const char* s)
{
    size_t len = 0;

    // Process four characters at a time if possible
    for (; *s && *(s + 1) && *(s + 2) && *(s + 3); s += 4)
    {
        len += 4;
    }

    // Handle the remaining characters
    while (*s)
    {
        len++;
        s++;
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

// Reverse string
void strrev(char* str)
{
    int length = strlen(str);
    int start = 0;
    int end = length - 1;

    while (start < end)
    {
        // Swap characters
        char temp = str[start];
        str[start] = str[end];
        str[end] = temp;

        // Move towards the middle
        start++;
        end--;
    }
}

// Integer to string
void itoa(int num, char* str, int base)
{
    int i = 0;
    int isNegative = 0;

    // Handle 0 explicitly
    if (num == 0)
    {
        str[i++] = '0';
        str[i] = '\0';
        return;
    }

    // Handle negative numbers for base 10
    if (num < 0 && base == 10)
    {
        isNegative = 1;
        num = -num;  // Make negetive to positive
    }

    // Process individual digits
    while (num != 0)
    {
        int rem = num % base;
        str[i++] = (rem > 9) ? (rem - 10) + 'a' : rem + '0';
        num = num / base;
    }

    // Append '-' of the number is negetive
    if (isNegative)
        str[i++] = '-';

    str[i] = '\0';  // Null-terminate the string

    // Reverse the string
    strrev(str);
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

void puts(const char* s)
{
    while (*s)
    {
        putchar(*s++);
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

char* strcpy(char* dest, const char* src)
{
    char* ret = dest;
    while (*src)
    {
        *dest++ = *src++;
    }
    *dest = "\0";
    return ret;
}

char* strcat(char* dest, const char* src)
{
    char* ptr = dest + strlen(dest);
    while (*src)
    {
        *ptr++ = *src++;
    }
    *ptr = '\0';
    return dest;
}

void fz_strcpy(char* src, char* dest)
{
    if (dest == NULL)
    {
        dest = ptr;
    }
    while (*src)
    {
        *dest++ = *src++;
    }
    ptr = dest;
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
    if (dest == NULL)
    {
        dest = ptr;
    }

    // d("Destination", dest);

    while (*src && max--)
    {
        *dest++ = *src++;
    }

    ptr = dest;
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

////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////
////////////////////////////////////////////////

void printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);

    for (const char* p = format; *p != '\0'; p++)
    {
        if (*p == '%' && *(p + 1) != '\0')
        {
            p++;  // Move to the format specifier
            switch (*p)
            {
                case 'd':
                    int i = va_args(args, int);
                    char buffer[20];
                    itoa(i, buffer, 10);  // Convert integer into string
                    puts(buffer);
                    break;
                case 's':
                    char* s = va_args(args, char*);
                    puts(s);
                    break;
                case 'c':
                    int c = va_args(args, int);  // I don't know more about it
                    putchar(c);
                    break;
                default:
                    putchar('%');
                    putchar(*p);
                    break;
            }
        }
        else
            putchar(*p);  // Print the character
    }

    va_end(args);
}