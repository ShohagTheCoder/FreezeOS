[BITS 32]
[org 0xa000]

; section .bss
; stack resb 10

section .text
kernel_kernel:
    ; ; Setup stack
    ; mov eax, stack + 10
    ; mov ebp, eax
    ; mov esp, ebp

    ; Find the kernel
    call load_kernel

    ; jmp $

    ; Jump to kernel location
    call kernel_location

    ; mov ecx, 104
    ; mov ebx, kernel_location
    ; call read_sector
    ; mov ecx, 105
    ; mov ebx, kernel_location + 512
    ; call read_sector



    ; mov si, kernel_location
    ; mov al, [si]
    ; mov byte [0xb8100], al
    ; mov byte [0xb8101], 0x5f
    ; mov al, [si + 1]
    ; mov byte [0xb8102], al
    ; mov byte [0xb8103], 0x5f
    ; mov al, [si + 2]
    ; mov byte [0xb8104], al
    ; mov byte [0xb8105], 0x5f
    ; mov al, [si + 3]
    ; mov byte [0xb8106], al
    ; mov byte [0xb8107], 0x5f
    ; mov al, [si + 4]
    ; mov byte [0xb8108], al
    ; mov byte [0xb8109], 0x5f
    ; mov al, [si + 5]
    ; mov byte [0xb810a], al
    ; mov byte [0xb810b], 0x5f

    mov esi, kernel_location + 0x800
    mov ecx, 1000
    call print_string;

    ; mov byte [0xb80f6], al
    ; mov byte [0xb80f7], 0x5f

    jmp $

print_string:
    ; mov edi, 0
    add ecx, 0xb8000

.print_loop:
    lodsb
    mov byte [ecx], al
    mov byte [ecx + 1], 0x4f
    cmp al, 0
    je .print_end
    add edi, 1
    add ecx, 2
    jmp .print_loop

.print_end:
    ret


section .data
; kernel location
fat_location       equ 0x9000
kernel_location    equ 0x100000

; kernel name
kernel_name db "KERNEL  BIN", 0
; kernel_name db "ONE     TXT", 0


; File system data
BytesPerSector      dw 512           ; Bytes per sector
SectorsPerCluster   db 4             ; Sectors per cluster
ReservedSectors     dw 4             ; Reserved sectors
NumberOfFATs        db 2             ; Number of FATs
RootEntries         dw 512           ; Root directory entries
MediaDescriptor     db 0xF8          ; Media descriptor
SectorsPerFAT       dw 20             ; Sectors per FAT
HiddenSectors       dd 0             ; Hidden sectors

; Includes
%include "arch/x86/helpers/find_file.asm"
%include "arch/x86/helpers/load_kernel.asm"
%include "arch/x86/helpers/read_sector.asm"