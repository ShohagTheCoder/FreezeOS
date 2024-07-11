[BITS 16]
[ORG 0x7C00]

; Define the BIOS Parameter Block (BPB)
jmp short start
nop
OEMLabel            db 'MSDOS5.0'    ; OEM Name
BytesPerSector      dw 512           ; Bytes per sector
SectorsPerCluster   db 4             ; Sectors per cluster
ReservedSectors     dw 4             ; Reserved sectors
NumberOfFATs        db 2             ; Number of FATs
RootEntries         dw 512           ; Root directory entries
TotalSectors        dw 20480          ; Total sectors (for 1.44MB floppy)
MediaDescriptor     db 0xF8          ; Media descriptor
SectorsPerFAT       dw 20             ; Sectors per FAT
SectorsPerTrack     dw 32            ; Sectors per track
NumberOfHeads       dw 2             ; Number of heads
HiddenSectors       dd 0             ; Hidden sectors
TotalSectorsBig     dd 0             ; Total sectors (if TotalSectors is 0)
DriveNumber         db 0             ; Drive number
Reserved1           db 0             ; Reserved
BootSignature       db 0x29          ; Extended boot signature
VolumeID            dd 0x12345678    ; Volume ID
VolumeLabel         db 'NO NAME    ' ; Volume label
FileSystemType      db 'FAT16   '    ; File system type

; Kernel location
fat_location        dw 0xf000
kernel_location     dw 0x9000

start:
    ; Setup stacks
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Call load fat
    call load_fat

    ; Load the kernel
    call load_kernel

    mov si, [kernel_location]
    call print_string

    jmp $

load_fat:
    mov dl, 5
    mov si, [fat_location]
    call read_sector
    mov ax, [si]
    cmp ax, 0xfff8
    je .fat_Loaded

    ; Faild to load the fat
    mov ah, 0x0e
    mov al, 'F'
    int 0x10
    ret

.fat_Loaded:
    mov ah, 0x0e
    mov al, 'L'
    int 0x10
    ret

print_string:
    mov ah, 0x0e

.print_loop:
    lodsb
    cmp al, 0
    je .print_end
    int 0x10
    jmp .print_loop

.print_end:
    ret

load_kernel:
    call find_file

    ; Move the next cluster number on cl register
    mov cx, ax
    call load_cluster

    ret

load_cluster:
    mov ah, 0x0e
    mov al, 'K'
    int 0x10

    mov dx, [kernel_location]
    
    call load_data

    mov si, 0xf000
    mov ax, 0
    mov bx, 0

    mov ax, 2
    mov bx, cx
    mul bx

    add si, ax
    mov cx, [si]
    sub si, ax
    cmp cx, 0xffff
    ; No cluster left
    je kernel_loaded

    mov ax, 0
    mov bx, 0
    mov ax, [SectorsPerCluster]
    mov bx, 512
    mul bx

    ; Move offset to current position
    add dx, bx

    mov ah, 0x0e
    mov al, 'J'
    int 0x10
    
    jmp load_cluster
load_data:
    mov ax, dx
    mov si, ax
    mov dx, cx
    add dx, 18
    mov dh, 1
    call read_sector
    mov dx, ax
    ret

kernel_loaded:
    mov ah, 0x0e
    mov al, 'S'
    int 0x10
    ret

disk_error:
    mov ah, 0x0e
    mov al, 'D'
    int 0x10
    hlt

find_file:

    mov ax, [SectorsPerFAT]
    mov bx, [NumberOfFATs]
    mul bx
    mov cl, al
    add cl, [ReservedSectors]
    add cl, 1
    mov dl, cl

    mov si, 0x7e00
    call read_sector
    mov si, 0x7e20

    mov cx, RootEntries
find_file_loop:
    ; Check if this is the file we are looking for
    mov di, kernel_name
    mov cx, 11
    repe cmpsb
    je file_found

    ; Move to next directory entry
    mov ax, 53
    add ax, cx
    add si, ax
    loop find_file_loop

file_not_found:
    mov ah, 0x0e
    mov al, 'N'
    int 0x10
    jmp $

file_found:
    ; Get first cluster
    mov ah, 0x0e
    mov al, 'X'
    int 0x10
    mov ax, [si + 15]
    ret

read_sector:
    ; mov ah, 0x0e
    ; mov al, 'R'
    ; int 0x10
    ; Read sector from disk
    pusha
    mov ah, 0x02
    mov al, [SectorsPerCluster]          ; Read one sector
    mov ch, 0          ; Cylinder number
    mov cl, dl         ; Sector number
    mov dl, 0x80           ; Drive number
    mov bx, si         ; Buffer to read into
    int 0x13
    ; jc disk_error
    popa
    ret


kernel_name db 'ONE     TXT', 0
; end_of_code db '-END OF CODE', 0

TIMES 510-($-$$) db 0
DW 0xAA55
