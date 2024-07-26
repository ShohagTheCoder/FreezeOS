#include "includes/console.h"
#include "includes/disk.h"
#include "includes/fs.h"
#include "includes/idt.h"
#include "includes/keyboard.h"
#include "includes/memory.h"
#include "includes/shell.h"
#include "includes/string.h"

#define SECTOR_SIZE 512

/**
 * @brief this kernel function is the heart of FreezeOS
 *
 */
void kernel()
{
    load_root_entries();
    load_fat();

    // Initialize interrupts
    idt_init();
    mask_all_irqs();
    keyboard_init();
    asm volatile("sti");

    // Initialize shell
    shell_init();

    fz_create_file("HI MY", "Bi");
}