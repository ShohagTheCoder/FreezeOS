#include "includes/console.h"
#include "includes/keyboard.h"
#include "includes/string.h"

// typedef struct
// {
//     // data segment selector
//     uint32_t ds;
//     // general purpose registers pushed by pusha
//     uint32_t edi, esi, ebp, esp, ebx, edx, ecx, eax;
//     // pushed by isr procedure
//     uint32_t int_no, err_code;
//     // pushed by CPU automatically
//     uint32_t eip, cs, eflags, useresp, ss;
// } registers_t;

// kernel.c
void main()
{
    // Clear the screen
    clear_screen();

    // init_idt();

    // asm volatile("sti");

    char *str = "Hello Shohag Ahmed!\n";
    char *google = "GOOGLE\n";

    print_str(str);
    print_int(-1234567890);
    reverse_str(google);
    print_str(google);

    while (1)
    {
        // Infinite loop
    }
}

// char *exception_messages[] = {"Division by zero", "Debug", "Reserved"};

// void isr_handler(registers_t *r)
// {
//     print_string(exception_messages[r->int_no]);
//     print_nl();
// }