; protected_mode.asm
[BITS 32]
; Start of protected mode
protected_mode:
    ; Set up segment registers with the data segment selector
    mov ax, DATA_SEG     ; Load the data segment selector into AX
    mov ds, ax           ; Set DS to the data segment
    mov es, ax           ; Set ES to the data segment
    mov fs, ax           ; Set FS to the data segment
    mov gs, ax           ; Set GS to the data segment
    mov ss, ax           ; Set SS to the data segment

    ; Jump to the kernel's starting location
    jmp kernel_location ; Call the kernel's entry point