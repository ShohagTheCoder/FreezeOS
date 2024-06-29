#include "includes/drivers/console.h"
#include "includes/lib/string.h"

// kernel.c
void main()
{
    // Clear the screen
    clear_screen();

    char *str = "Hello Shohag Ahmed!\n";
    char *pc = "Hello Mr. PC!";

    char *dc = "GOOGLE\n";

    print_str(str);

    print_str(pc);

    print_int(-1234567890);

    reverse_str(dc);

    print_str(dc);

    while (1)
    {
        // Infinite loop
    }
}