#include "../includes/string.h"
#include "../includes/console.h"

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

int str_cmp(char* str1, char* str2)
{
    while (*str1 && (*str1 == *str2))
    {
        str1++;
        str2++;
    }
    // Check if both strings have reached the end simultaneously (both are '\0')
    if (*str1 == '\0' && *str2 == '\0')
    {
        return 1;  // Strings match
    }
    else
    {
        return 0;  // Strings do not match
    }
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