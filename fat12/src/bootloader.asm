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
BootCode:

start:
    ; Setup stacks
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00
    
    ; Call load fat
    call load_fat

    ; Load the kernel
    ; call load_kernel

    mov ax, [SectorsPerFAT]
    mov bx, [NumberOfFATs]
    mul bx
    mov cl, al
    add cl, [ReservedSectors]
    mov ax, [RootEntries]
    mov bx, 32
    mul bx
    mov bx, 512
    div bx
    add cl, al
    add cl, 5
    cmp cl, 63

    jg next_head

    jmp current_head

next_head:
    sub cl, 63
    add dh, 1

current_head:
    mov dl, cl
    ; mov dl, 18
    mov si, 0x7e00
    call read_sector

    ; mov bx, 0x7e00
    ; mov ah, 0x02 		; BIOS read sector function
    ; mov al, 1
    ; mov cl, 18
    ; mov ch, 0
	; mov dh, 1
    ; mov dl, 0x80
    ; int 0x13

    ; jc disk_error


    ; mov si, 0x7e00
    ; call print_string
    ; mov al, byte [0x7e01]
    ; int 0x10

    ; Jump to kernel
    ; jmp 0x1000:0000
    ; jmp 0x7e00
    ; jmp [si]



    jmp $

load_fat:
    mov dl, 5
    mov si, 0xf000
    call read_sector
    mov ax, [si]
    cmp ax, 0xfff8
    je .fat_Loaded

    ; Faild to load the fat
    mov ah, 0x0e
    mov al, 'F'
    int 0x10
    ret

.fat_Loaded
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
    ; mov ah, 0x0e
    ; mov al, '2'
    ; int 0x10
    ; Read FAT12 BPB
    ; mov si, kernel_name
    mov si, 0x7e00
    call find_file
    ; mov ah, 0x0e
    ; mov al, '3'
    ; int 0x10

    ; Load file clusters
    mov cx, ax  ; Start cluster (returned from find_file)
    mov si, 0x8e00
    mov dl, 2
    call read_sector

load_cluster:
    ; mov ah, 0x0e
    ; mov al, '4'
    ; int 0x10
    ; call get_next_cluster
    ; cmp ax, 0xFFF8
    ; jae kernel_loaded

    ; Read cluster into memory
    ; mov ax, 0x1000  ; Segment to load kernel
    ; mov es, ax
    ; mov bx, 0x7f00
    ; mov ah, 0x0e
    ; mov al, cl
    ; int 0x10
    ; mov al, ch
    ; int 0x10

    add ax, 0x20
    mov dx, ax
    mov si, 0x7f00
    call read_sector

    ; call read_cluster
    mov ah, 0x0e
    mov al, [si]
    int 0x10
    call print_string

    ; call get_next_cluster
    
    jmp $

    ; jmp load_cluster

kernel_loaded:
    ; mov ah, 0x0e
    ; mov al, 'S'
    ; int 0x10
    ret

disk_error:
    mov ah, 0x0e
    mov al, 'D'
    int 0x10
    hlt

find_file:
    ; Load root directory
    ; mov dx, [ReservedSectors]
    ; mov ax, [NumberOfFATs]
    ; mov bx, [SectorsPerFAT]
    ; mul bx
    ; add dx, ax
    ; add dx, 1
    ; mov ax, dx
    ; mov bx, 0x7e00
    ; mov dx, ReservedSectors + (NumberOfFATs * SectorsPerFAT) + 1
    mov si, 0x7e00
    call read_sector


    ; mov ah, 0x0e
    ; mov al, byte [0x7e42]
    ; int 0x10
    ; mov al, byte [0x7e41]
    ; int 0x10
    ; mov al, byte [0x7e42]
    ; int 0x10
    ; mov al, byte [0x7e43]
    ; int 0x10
    ; mov al, byte [0x7e44]
    ; int 0x10
    ; mov al, byte [0x7e45]
    ; int 0x10

    ; mov bx, 0x7e00
    ; mov al, 1
    ; mov cl, 20
    ; mov ah, 0x02 		; BIOS read sector function
    ; mov ch, 0
	; mov dh, 0
    ; int 0x13

    ; mov ah, 0x0e
    ; mov al, byte [0x7e00]
    ; int 0x10

    ; jmp $


    ; Traverse root directory to find the file
    ; mov si, 0x7E00
    mov cx, RootEntries
find_file_loop:
    ; Check for end of directory
    ; mov al, byte [si]
    add si, 0x40
    ; cmp al, 0x00
    ; je file_not_found

    ; mov ah, 0x0e
    ; mov al, byte [si + 64]
    ; int 0x10
    ; mov al, byte [si + 65]
    ; int 0x10
    ; mov al, byte [si + 66]
    ; int 0x10
    ; mov al, byte [si + 67]
    ; int 0x10

    ; jmp $

    ; Check if this is the file we are looking for
    mov di, kernel_name
    mov cx, 11
    repe cmpsb
    je file_found

    ; Move to next directory entry
    add si, 64
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
    mov al, 'X'
    int 0x10
    mov ax, [si + 15]
    ret

get_next_cluster:
    ; mov ah, 0x0e
    ; mov al, '5'
    ; int 0x10
    ; Calculate FAT entry
    ; xor dx, dx
    ; mov di, cx  ; cx contains the current cluster number
    ; mov bx, di
    ; shr bx, 1
    ; add di, bx

    add cx, 2 ; fat start location
    mov al, [si]
    add cx, 1
    mov ah, [si]


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
    mov dl, 0x80           ; Drive number
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
    ; xor dx, dx
    ; mov ax, cx  ; cx contains the current cluster number
    ; sub ax, 2
    ; mov bx, [SectorsPerCluster]
    ; mul bx
    ; add ax, 0x21  ; Add the first data sector number
    ; mov bx, 0x7f00
    mov dl, al
    mov si, 0x7f00
    call read_sector
    ret

kernel_name db 'TWO     TXT', 0

TIMES 510-($-$$) db 0
DW 0xAA55
