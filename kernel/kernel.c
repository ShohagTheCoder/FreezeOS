#include "includes/drivers/console.h"
#include "includes/lib/string.h"

// kernel.c
void main()
{
    // Clear the screen
    clear_screen();

    const char *str = "Hello Shohag Ahmed!";
    const char *pc = "Hello Mr. PC!";

    print_str(str);
    print_str(str);
    print_str(pc);

    print_int(1234567890);

    while (1)
    {
        // Infinite loop
    }
}