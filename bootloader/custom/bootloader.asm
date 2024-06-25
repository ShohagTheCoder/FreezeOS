[BITS 16]
[ORG 0x7C00]

; Variables
kernel_location equ 0x1000
; boot_disk: db 0
; mov [boot_disk], dl

; Messages
hello_world db 'Hello world! Go', 0x0d, 0x0a, 0

; Starting point
start:
	; Print hello word
	mov si, hello_world
	call print_string

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

; Print once
print_once:
    mov ah, 0x0e
	lodsb
	int 0x10
	ret
 
; Print pring
print_string:
    mov ah, 0x0e

.start_printing:
    lodsb
    cmp al, 0
    je .print_done
    int 0x10
    jmp .start_printing

; Work has done go back
.print_done:
	xor si, si
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
; Starting point of protected mode
start_protected_mode:
	;Set up segment registers
	mov ax, 0x10
	mov ds, ax
	mov es, ax
	mov ss, ax
	mov sp, 0x7c00 		; Set stack pointer

	; Jump to kernel
	jmp kernel_location

; Bootloader healpers
times 510-($-$$) db 0   ; Fill the rest of the boot sector with zeros
dw 0xAA55               ; Boot signature