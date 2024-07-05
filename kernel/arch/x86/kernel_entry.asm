; kernel_entry.asm
[bits 32]

section .text
; Extern functions
extern kernel
extern keyboard_handler

; Kernel entry point
global kernel_entry
kernel_entry:

    call kernel         ; Call the kernel

    jmp $

; irq1 handler
global irq1_handler
irq1_handler:
    cli
    pushad

    call keyboard_handler ; Call keyboard handler to hangle the interrupt

    mov al, 0x20    ; End of enterrupt (EOI) command code
    out 0x20, al    ; Send to master pic

    popad
    sti
    iretd