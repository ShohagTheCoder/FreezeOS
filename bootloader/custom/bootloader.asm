[BITS 16]
[ORG 0x7C00]

; Variables
kernel_location equ 0x1000

; Starting point
start:
    ; Load the kernel
    call load_the_kernel

	; Prepare for protected mode
    call go_to_protected_mode

; Read from disk
read_from_disk:
    mov ah, 0x02 		; BIOS read sector function
    mov ch, 0
	mov dh, 0
    int 0x13
    ret

; Load the kernel
load_the_kernel:
	mov bx, kernel_location		; Where to load the data
	mov cl, 2 					; Sector number to read (destination)
	mov al, 10					; Numbers of sector to read

	; Call read from disk
	call read_from_disk
	ret

; GDT descriptor
GDT_start:
	null_descriptor:
		dd 0
		dd 0
	code_descriptor:
		dw 0xffff
		dw 0
		db 0
		db 0x9a
		db 0xcf
		db 0
	data_descriptor:
		dw 0xffff
		dw 0
		db 0
		db 0x92
		db 0xcf
		db 0
GDT_end:

GDT_descriptor:
	dw GDT_end - GDT_start - 1
	dd GDT_start

CODE_SEG equ code_descriptor - GDT_start
DATA_SEG equ data_descriptor - GDT_start

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

; Start of protected mode code
start_protected_mode:
    ; Set up segment registers with the data segment selector
    mov ax, DATA_SEG     ; Load the data segment selector into AX
    mov ds, ax           ; Set DS to the data segment
    mov es, ax           ; Set ES to the data segment
    mov fs, ax           ; Set FS to the data segment
    mov gs, ax           ; Set GS to the data segment
    mov ss, ax           ; Set SS to the data segment

    ; Initialize the stack pointer
    mov esp, 0x7c00      ; Set ESP to the start of the stack (top of memory)

    ; Jump to the kernel's starting location
    call kernel_location ; Call the kernel's entry point

; Infinite loop to hang the system
hang:
    hlt                 ; Halt the CPU
    jmp hang            ; Jump to the hang label, creating an infinite loop


; Bootloader healpers
times 510-($-$$) db 0   ; Fill the rest of the boot sector with zeros
dw 0xAA55               ; Boot signature