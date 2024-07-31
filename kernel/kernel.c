#include "includes/console.h"
#include "includes/disk.h"
#include "includes/fs.h"
#include "includes/idt.h"
#include "includes/keyboard.h"
#include "includes/logs.h"
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
    // Initilize memory and file system
    init_dynamic_mem();
    load_root_entries();
    load_fat();

    // Initialize interrupts
    idt_init();
    mask_all_irqs();
    keyboard_init();
    asm volatile("sti");

    // Initialize shell
    shell_init();

    // log_init();

    // DirEntry_t error = find_file("errors", "txt");

    // print_str(error.name);

    // ERROR("kernel.c", "kernel_main", "Reading from file is failed");
    // WARN("kernel.c", "kernel_main", "Reading from file is failed");
    // INFO("kernel.c", "kernel_main", "Reading from file is failed");

    int i = 2644;
    char* name = "shohag ahmed!";

    printf("There is a man called %s. He is %d years old.", name, i);
}