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
    // Clear the screen and print welcome
    clear_screen();
    print_str("--------------------------------------------------------------------------------");
    print_str("Welcome to FreezeOS v1.0.3");
    put_nl();
    print_str("--------------------------------------------------------------------------------");
    print_str("> ");

    // Initialize interrupts
    idt_init();
    mask_all_irqs();
    keyboard_init();
    asm volatile("sti");

    while (1)
    {
        // Infinite loop
    }
}