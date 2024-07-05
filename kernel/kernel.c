#include "includes/console.h"
#include "includes/idt.h"
#include "includes/keyboard.h"
#include "includes/shell.h"
#include "includes/string.h"

/**
 * @brief this kernel function is the heart of FreezeOS
 *
 */
void kernel()
{
    // Initialize interrupts
    idt_init();
    mask_all_irqs();
    keyboard_init();
    asm volatile("sti");

    // Initialize shell
    shell_init();

    while (1)
    {
        // Infinite loop
    }
}