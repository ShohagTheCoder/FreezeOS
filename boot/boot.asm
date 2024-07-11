; boot.asm
[BITS 16]
[org 0x7c00]

; Starting point
start:
    ; Load the kernel
    call load_the_kernel

	; Prepare for protected mode
    call jmp_to_pm

; Load the kernel
load_the_kernel:
	mov bx, kernel_location		; Where to load the data
	mov cl, 2 					; Sector number to read (destination)
	mov al, 40					; Numbers of sector to read

	; Call read from disk
	call read_disk 
	ret

; Variables
kernel_location equ 0x7e00

; Infinite loop to hang the system
hang:
    hlt                 ; Halt the CPU
    jmp hang            ; Jump to the hang label, creating an infinite loop

; Include helper files
%include "helpers/read_disk.asm"
%include "helpers/gdt.asm"
%include "helpers/jmp_to_pm.asm"
%include "helpers/protected_mode.asm"

; Bootloader healper and signature
times 510-($-$$) db 0   ; Fill the rest of the boot sector with zeros
dw 0xAA55               ; Boot signature
