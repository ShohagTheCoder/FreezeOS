#include "includes/drivers/console.h"
#include "includes/lib/string.h"

// kernel.c
void main()
{
    // Clear the screen
    clear_screen();

    const char *str = "Hello Shohag Ahmed!";

    print_str(str);

    print_int(strlen(str));

    while (1)
    {
        // Infinite loop
    }
}
