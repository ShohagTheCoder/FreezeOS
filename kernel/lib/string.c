#include "../includes/lib/string.h"
#include "../includes/drivers/console.h"

// Get string len
size_t strlen(const char *str)
{
    size_t len = 0;
    while (str[len] != '\0')
    {
        len++;
    }

    return len;
}

// Function to convert an integer to its ASCII representation
void int_to_ascii(int n, char str[])
{
    int is_negative = 0;

    // Handle negative numbers
    if (n < 0)
    {
        is_negative = 1;
        n = -n;
    }

    int len = count_digits(n);

    str[len] = '\0'; // Null-terminate the string

    // Add negative sign if the number was negative
    if (is_negative)

    {
        str[0] = '-';
        len++;
    }

    // Generate digits in reverse order
    do
    {
        str[--len] = n % 10 + '0'; // Convert each digit to ASCII
        n /= 10;
    } while (n > 0);
}

// void int_to_ascii(int n, char str[])
// {
//     int i = 0;
//     int is_negative = 0;

//     // Handle negative numbers
//     if (n < 0)
//     {
//         is_negative = 1;
//         n = -n;
//     }

//     int len = strlen(str);

//     // Generate digits in reverse order
//     do
//     {
//         str[i++] = n % 10 + '0'; // Convert each digit to ASCII
//         n /= 10;
//     } while (n > 0);

//     // Add negative sign if the number was negative
//     if (is_negative)
//     {
//         str[i++] = '-';
//     }

//     str[i] = '\0'; // Null-terminate the string
// }

/**
 * Print string function
 */
void print_str(const char *str)
{
    while (*str)
    {
        putchar(*str++);
    }
}

// Example implementation of print_int assuming screen output functions exist
void print_int(int n)
{
    char str[20]; // Assuming a maximum of 20 digits for the number
    int_to_ascii(n, str);

    // Assuming you have a function to print strings to the screen
    print_str(str); // Replace with your screen output function
}

void reverse_str(char str[])
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