; load_kernel.asm
load_kernel:
    ; Find the file from root directory
    call find_file

    ; Move the cluster number on cx register
    mov cx, ax

    ; Call load cluster to load all clusters of current file
    call load_cluster

    ret

load_cluster:
    ; Load the cluster data to memory
    call load_data

    mov si, [fat_location]
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
    mov dx, [kernel_offset]
    add dx, ax
    mov [kernel_offset], dx

    jmp load_cluster
load_data:
    mov ah, 0x0e
    mov al, "D"
    int 0x10
    
    mov al, cl
    mov bl, 4
    mul bl
    mov dl, al
    add dl, 6
    mov dh, 1
    mov si, [kernel_offset]
    call read_sector
    ret

kernel_loaded:
    mov ah, 0x0e
    mov al, "S"
    int 0x10
    ret