#include "../includes/string.h"
#include "../includes/console.h"
#include "../includes/memory.h"

// Counts the number of character in the string.
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

// Remove the last caharcter of a string
void pop(char* str)
{
    size_t len = strlen(str);

    if (len > 0)
    {
        str[len - 1] = '\0';  // Remove the last character
    }
}

// Push a new character at the end of the string
void push(char* str, char c)
{
    size_t len = strlen(str);
    str[len] = c;
    str[len + 1] = '\0';
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

// Compare string for maximum n times
int strncmp(const char* a, const char* b, size_t n)
{
    while (n && *a && (*a == *b))
    {
        a++;
        b++;
        n--;
    }

    if (n == 0)
    {
        return 0;
    }

    return *(unsigned char*)a - *(unsigned char*)b;
}

// Print a string into console
void puts(const char* s)
{
    while (*s)
    {
        putchar(*s++);
    }
}

// Print characters n number of times
void putns(const char* str, size_t n)
{
    while (n--)
    {
        putchar(*str++);
    }
}

// Example implementation of print_int assuming screen output functions exist
void puti(int n)
{
    // Create a empty array with length equal to the counts of the number to
    // hold the returen string
    char str[count_digits(n)];
    itoa(n, str, 10);

    // Assuming you have a function to print strings to the screen
    puts(str);  // Replace with your screen output function
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

    // Handle the special case of 0
    if (n == 0)
    {
        return 1;
    }

    // Handle negative ns
    if (n < 0)
    {
        n = -n;
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
    *dest = '\0';
    return ret;
}

char* strncpy(char* dest, const char* src, size_t n)
{
    size_t i;
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

// Function to extract a substring
char* substr(const char* src, int start, int length)
{
    int src_len = strlen(src);
    // Validate the input
    if (src == NULL || start < 0 || length < 0 || start >= src_len)
    {
        return NULL;
    }

    // Adjust the length if its goes beyond the src string
    if (start + length > src_len)
    {
        length = src_len - start;
    }

    // Allocase memory for the substring
    char* result = (char*)malloc(length + 1);
    if (result == NULL)
    {
        puts("Memory allocation faild\n");
        return NULL;
    }

    // Copy the substring
    strncpy(result, src, length);
    result[length] = '\0';  // Null-terminate the string

    return result;
}

void to_uppercase(char* str)
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

void to_lowercase(char* str)
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

// find the first occurance of the c and return the index
char* strchr(const char* str, int c)
{
    while (*str)  // Loop untill we reach the end of the string
    {
        if (*str == (char)c)
        {
            return (char*)str;
        }
        str++;
    }

    if (*str == (char)c)  // Check if the character is at the end of the string
    {
        return (char*)str;
    }

    return NULL;  // If not match return NULL
}

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

char* strtok(char* str, const char* delim)
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

void strsplit(char* str, char** pointers, char* delim)
{
    char* token = strtok(str, delim);
    int count = 0;

    while (token != NULL)
    {
        pointers[count] = token;
        count++;
        token = strtok(NULL, delim);
    }
}

char* format_string(const char* format, va_list args)
{
    // Allocate memory space for the formated string
    char* str = malloc(MAX_BUFFER_SIZE);
    if (str == NULL)
    {
        return NULL;
    }

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
                    strcat(str, buffer);
                    break;
                case 's':
                    char* s = va_args(args, char*);
                    strcat(str, s);
                    break;
                case 'c':
                    int c = va_args(args, int);  // I don't know more about it
                    push(str, c);
                    break;
                default:
                    push(str, '%');
                    push(str, *p);
                    break;
            }
        }
        else
        {
            push(str, *p);  // Print the character
        }
    }

    return str;
}

// Formate a string and print into the console
void printf(const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char* str = format_string(format, args);
    if (str != NULL)
    {
        puts(str);
        free(str);
    }
    va_end(args);
}

// Formate a string and save in buffer
void sprintf(char* buffer, const char* format, ...)
{
    va_list args;
    va_start(args, format);
    char* str = format_string(format, args);
    if (str != NULL)
    {
        strncpy(buffer, str, MAX_BUFFER_SIZE);
        free(str);
    }
    va_end(args);
}