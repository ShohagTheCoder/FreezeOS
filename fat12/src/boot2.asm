[BITS 16]
[ORG 0x7C00]

; Define the BIOS Parameter Block (BPB)
jmp short start
nop
OEMLabel            db 'MSDOS5.0'    ; OEM Name
BytesPerSector      dw 512           ; Bytes per sector
SectorsPerCluster   db 1             ; Sectors per cluster
ReservedSectors     dw 1             ; Reserved sectors
NumberOfFATs        db 2             ; Number of FATs
RootEntries         dw 224           ; Root directory entries
TotalSectors        dw 2880          ; Total sectors (for 1.44MB floppy)
MediaDescriptor     db 0xF0          ; Media descriptor
SectorsPerFAT       dw 9             ; Sectors per FAT
SectorsPerTrack     dw 18            ; Sectors per track
NumberOfHeads       dw 2             ; Number of heads
HiddenSectors       dd 0             ; Hidden sectors
TotalSectorsBig     dd 0             ; Total sectors (if TotalSectors is 0)
DriveNumber         db 0             ; Drive number
Reserved1           db 0             ; Reserved
BootSignature       db 0x29          ; Extended boot signature
VolumeID            dd 0x12345678    ; Volume ID
VolumeLabel         db 'NO NAME    ' ; Volume label
FileSystemType      db 'FAT12   '    ; File system type
BootCode:

start:
    ; mov ah, 0x0e
    ; mov al, '1'
    ; int 0x10
    ; Set up stack
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load the kernel
    call load_kernel

    ; mov ah, 0x0e
    ; mov al, '8'
    ; int 0x10

    ; Jump to kernel
    ; jmp 0x1000:0000
    ; jmp [si]
    hlt

load_kernel:
    ; mov ah, 0x0e
    ; mov al, '2'
    ; int 0x10
    ; Read FAT12 BPB
    mov si, kernel_name
    mov si, 0x7e00
    call find_file
    ; mov ah, 0x0e
    ; mov al, '3'
    ; int 0x10

    ; Load file clusters
    mov cx, ax  ; Start cluster (returned from find_file)
load_cluster:
    ; mov ah, 0x0e
    ; mov al, '4'
    ; int 0x10
    call get_next_cluster
    cmp ax, 0xFFF8
    jae kernel_loaded

    ; Read cluster into memory
    mov ax, 0x1000  ; Segment to load kernel
    mov es, ax
    mov bx, 0
    call read_cluster
    ; mov ah, 0x0e
    ; mov al, '7'
    ; int 0x10
    
    ; jmp $

    jmp load_cluster

kernel_loaded:
    ; mov ah, 0x0e
    ; mov al, 'S'
    ; int 0x10
    ret

disk_error:
    ; mov ah, 0x0e
    ; mov al, 'D'
    ; int 0x10
    hlt

find_file:
    ; Load root directory
    mov ax, [BytesPerSector]
    mov bx, [ReservedSectors]
    mul bx  ; Calculate root directory start sector
    mov dx, ax
    mov ax, [NumberOfFATs]
    mov bx, [SectorsPerFAT]
    mul bx
    add dx, ax  ; Add FAT sectors
    mov ax, dx
    mov bx, 0x7E00
    ; mov si, 0x7E00
    call read_sector

    ; Traverse root directory to find the file
    ; mov si, 0x7E00
    mov cx, RootEntries
find_file_loop:
    ; Check for end of directory
    ; mov al, byte [si]
    ; cmp al, 0x00
    ; je file_not_found

    ; Check if this is the file we are looking for
    mov di, kernel_name
    mov cx, 11
    repe cmpsb
    je file_found

    ; Move to next directory entry
    add si, 32
    loop find_file_loop

file_not_found:
    mov ah, 0x0e
    mov al, 'N'
    int 0x10
    mov ax, 0x1111
    jmp $

file_found:
    ; Get first cluster
    mov ah, 0x0e
    mov al, 'F'
    int 0x10
    mov ax, [si + 26]
    ret

get_next_cluster:
    ; mov ah, 0x0e
    ; mov al, '5'
    ; int 0x10
    ; Calculate FAT entry
    xor dx, dx
    mov di, cx  ; cx contains the current cluster number
    mov bx, di
    shr bx, 1
    add di, bx

    ; Read FAT sector
    mov ax, [BytesPerSector]
    mov bx, [ReservedSectors]
    mul bx
    add ax, di
    mov bx, 0x7E00
    call read_sector

    ; Get next cluster from FAT
    mov bx, word [0x7E00 + di]
    test cx, 1
    jz even_cluster
    shr bx, 4
    jmp get_next_done

even_cluster:
    ; mov ah, 0x0e
    ; mov al, '-'
    ; int 0x10
    and bx, 0x0FFF

get_next_done:
    ; mov ah, 0x0e
    ; mov al, '+'
    ; int 0x10
    mov ax, bx
    ret

read_sector:
    ; mov ah, 0x0e
    ; mov al, 'R'
    ; int 0x10
    ; Read sector from disk
    pusha
    mov ah, 0x02
    mov al, 1          ; Read one sector
    mov ch, 0          ; Cylinder number
    mov cl, dl         ; Sector number
    mov dh, 0          ; Head number
    mov dl, 0          ; Drive number
    mov bx, si         ; Buffer to read into
    int 0x13
    ; jc disk_error
    popa
    ret

read_cluster:
    ; mov ah, 0x0e
    ; mov al, '6'
    ; int 0x10
    ; Read cluster from disk
    ; Calculate sector from cluster
    xor dx, dx
    mov ax, cx  ; cx contains the current cluster number
    sub ax, 2
    mov bx, [SectorsPerCluster]
    mul bx
    add ax, 0x21  ; Add the first data sector number
    mov bx, 0x7E00
    call read_sector
    ret

kernel_name db 'TEXT    TXT', 0

TIMES 510-($-$$) db 0
DW 0xAA55
