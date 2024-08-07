#include <stdio.h>
#include <string.h>
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

    // char* t = malloc(1024);
    // sprintf(t, "File : %s, Function : %s, Message : %s", "Kernel.c", "MAIN", "FAILD to read
    // file"); puts(t);

    DirEntry_t file = find_file("errors.txt");
    // putns((char*)file.name, 8);
    // puti(file.size);
    fz_fappend(file, "Bangladesh!");

    // char* a = ">.>Hwllo world .>> go to bed";
    // char* b = ">.>";

    // a += strspn(a, b);

    // puts(a);

    // char* a = "Hello shohag.txt";
    // char* b = strtok(a, " ");
    // puts(a);
    // puts(strtok(NULL, "."));
    // puts(strtok(NULL, " "));

    // ERROR("Reading from file is failed");
    // WARN("kernel.c", "kernel_main", "Reading from file is failed");
    // INFO("kernel.c", "kernel_main", "Reading from file is failed");

    // execute_command("echo gooogle");
    // execute_command("echo gooogle > errors.txt");
    // printf("Hi this is %s. I'm %d years old", "Shohag Ahmed", 20);
}