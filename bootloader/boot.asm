[BITS 16]
[org 0x7c00]

; Starting point
start:
    ; Load the kernel
    call load_the_kernel

	; Prepare for protected mode
    call go_to_protected_mode

; Load the kernel
load_the_kernel:
	mov bx, kernel_location		; Where to load the data
	mov cl, 2 					; Sector number to read (destination)
	mov al, 40					; Numbers of sector to read

	; Call read from disk
	call read_disk
	ret

; Prepare for protected mode
go_to_protected_mode:
	cli

	lgdt [GDT_descriptor]
	mov eax, cr0
	or eax, 1
	mov cr0, eax	
	
	; Start protected mode
	jmp CODE_SEG:start_protected_mode

[BITS 32]

start_protected_mode:
    ; Set up segment registers with the data segment selector
    mov ax, DATA_SEG     ; Load the data segment selector into AX
    mov ds, ax           ; Set DS to the data segment
    mov es, ax           ; Set ES to the data segment
    mov fs, ax           ; Set FS to the data segment
    mov gs, ax           ; Set GS to the data segment
    mov ss, ax           ; Set SS to the data segment

    ; Jump to the kernel's starting location
    jmp kernel_location ; Call the kernel's entry point

; Variables
kernel_location equ 0x7e00

; Infinite loop to hang the system
hang:
    hlt                 ; Halt the CPU
    jmp hang            ; Jump to the hang label, creating an infinite loop

; Include helper files
%include "bootloader/helpers/read_disk.asm"
%include "bootloader/helpers/gdt.asm"

; Bootloader healper and signature
times 510-($-$$) db 0   ; Fill the rest of the boot sector with zeros
dw 0xAA55               ; Boot signature
