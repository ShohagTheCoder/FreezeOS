; kernel_entry.asm
[bits 32]

section .text
global kernel_entry
extern kernel

; Kernel entry point
kernel_entry:

    call kernel         ; Call the kernel

    hlt                 ; Halt the CPU