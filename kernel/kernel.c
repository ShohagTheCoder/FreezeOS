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

    log_init();

    // DirEntry_t file = find_file("one", "txt");
    // putns(file.name, 8);
    // puti(file.size);
    // fz_fappend(file, "Bangladesh!");

    // ERROR("kernel.c", "kernel_main", "Reading from file is failed");
    // WARN("kernel.c", "kernel_main", "Reading from file is failed");
    // INFO("kernel.c", "kernel_main", "Reading from file is failed");
}