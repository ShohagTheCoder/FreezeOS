#include "includes/drivers/console.h"

// Main entry point
void main()
{
    clear_screen();

    const char *str = "Hello, Shohag Ahmed! do";

    const char x = 'x';

    putchar(x);
    print_string(str);

    asm volatile("hlt");
}
