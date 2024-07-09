[BITS 16]
[ORG 0x7C00]

%define SECTOR_SIZE 512

start:
    ; Set up stack
    xor ax, ax
    mov ss, ax
    mov sp, 0x7C00

    ; Load the BPB
    mov ax, 0x0000
    mov es, ax
    mov bx, 0x7C00
    call read_sector

    ; Check for FAT12 signature
    cmp word [0x7DFE], 0xAA55
    jne disk_error

    ; Load the kernel
    call load_kernel

    ; Jump to kernel
    jmp 0x1000:0000

load_kernel:
    ; Read FAT12 BPB
    mov si, kernel_name
    call find_file

    ; Load file clusters
    mov cx, 0  ; Start cluster
load_cluster:
    call get_next_cluster
    cmp ax, 0xFFF8
    jae kernel_loaded

    ; Read cluster into memory
    mov ax, 0x1000  ; Segment to load kernel
    add ax, cx
    mov es, ax
    mov bx, 0
    call read_cluster

    jmp load_cluster

kernel_loaded:
    ret

disk_error:
    hlt

find_file:
    ; Implement FAT12 directory traversal to find file
    ; Load root directory
    mov ax, [bpb_bytes_per_sector]
    xor dx, dx
    mov dl, byte [bpb_reserved_sectors + 1]
    shl dx, 8
    add dl, byte [bpb_reserved_sectors]
    mul dx

    mov si, root_dir_start
    mov di, 0x0200  ; Segment to load root directory
    mov es, di
    mov bx, 0
    call read_sector

    ; Traverse root directory to find the file
    mov si, root_dir
    mov cx, root_entry_count
find_file_loop:
    ; Check for end of directory
    mov al, byte [si]
    cmp al, 0x00
    je file_not_found

    ; Check if this is the file we are looking for
    mov di, kernel_name
    mov cx, 11
    repe cmpsb
    je file_found

    ; Move to next directory entry
    add si, 32
    loop find_file_loop

file_not_found:
    hlt

file_found:
    ; Get first cluster
    mov ax, [si + 26]
    ret

get_next_cluster:
    ; Implement FAT12 FAT reading to get next cluster
    ; Calculate FAT entry
    xor dx, dx
    mov di, ax
    mov bx, di
    shr bx, 1
    add di, bx

    ; Read FAT sector
    mov ax, [bpb_bytes_per_sector]
    xor dx, dx
    mov dl, byte [bpb_reserved_sectors + 1]
    shl dx, 8
    add dl, byte [bpb_reserved_sectors]
    mul dx
    add di, ax
    mov si, fat_start
    call read_sector

    ; Get next cluster from FAT
    mov bx, ax
    test di, 1
    jz even_cluster
    shr bx, 4
    jmp get_next_done

even_cluster:
    and bx, 0x0FFF

get_next_done:
    ret

read_sector:
    ; Read sector from disk
    pusha
    mov ah, 0x02
    mov al, 1
    mov ch, 0
    mov cl, 2
    mov dh, 0
    int 0x13
    jc disk_error
    popa
    ret

read_cluster:
    ; Read cluster from disk
    ; Calculate sector from cluster
    xor dx, dx
    mov di, ax
    mov bx, di
    dec bx
    mul bx

    ; Read sectors
    mov cx, [bpb_sectors_per_cluster]
read_cluster_loop:
    call read_sector
    add bx, 1
    loop read_cluster_loop
    ret

kernel_name db 'KERNEL  BIN', 0

TIMES 510-($-$$) db 0
DW 0xAA55

%include "bpb.inc"
%include "root_dir.inc"
%include "fat.inc"
