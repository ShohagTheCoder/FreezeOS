#include "includes/console.h"
#include "includes/idt.h"
#include "includes/keyboard.h"
#include "includes/string.h"

/**
 * @brief this kernel function is the heart of FreezeOS
 *
 */
void kernel()
{
    // Clear the screen
    clear_screen();

    init_idt();
    mask_all_irqs();
    asm volatile("sti");

    char* str = "Hello Shohag Ahmed!\n";
    char* google = "GOOGLE\n";

    print_str(str);
    print_int(-1234567890);
    reverse_str(google);
    print_str(google);

    while (1)
    {
        // Infinite loop
    }
}