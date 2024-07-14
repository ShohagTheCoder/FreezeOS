; load_kernel.asm
section .text
load_kernel:
    ; Find the file from root directory
    call find_file

    ; Move the cluster number on cx register
    ; add eax, 1
    mov ecx, eax
    mov esi, kernel_location

    ; Call load cluster to load all clusters of current file
    call load_cluster

    ret

load_cluster:
    call load_data
    push esi

    mov esi, fat_location
    xor eax, eax
    xor ebx, ebx
    mov eax, 2
    mov ebx, ecx
    mul ebx

    add esi, eax
    mov cx, [si]
    cmp cx, 0xffff
    ; No cluster left
    je kernel_loaded

    mov eax, ecx
    ; mov eax, 10
    pop esi
    call load_data
    ; add esi, 2048
    ; call load_data

    jmp load_cluster
    ; ret
load_data:
    pusha
    mov edi, 0

    call get_sector_number
    mov ecx, ebx

.load_data_loop:
    mov ebx, esi
    call read_sector
    add ecx, 1
    add esi, 512

    ; Loop section
    cmp edi, 3
    je .load_data_done

    add edi, 1
    jmp .load_data_loop
    
.load_data_done:
    popa
    ret

kernel_loaded:
    mov byte [0xb80fa], 'Z'
    mov byte [0xb80fb], 0x2f

    mov si, 0xf000
    mov al, [si]
    mov byte [0xb8100], al
    mov byte [0xb8101], 0x5f
    mov al, [si + 1]
    mov byte [0xb8102], al
    mov byte [0xb8103], 0x5f
    mov al, [si + 2]
    mov byte [0xb8104], al
    mov byte [0xb8105], 0x5f
    mov al, [si + 3]
    mov byte [0xb8106], al
    mov byte [0xb8107], 0x5f
    mov al, [si + 4]
    mov byte [0xb8108], al
    mov byte [0xb8109], 0x5f
    mov al, [si + 5]
    mov byte [0xb810a], al
    mov byte [0xb810b], 0x5f

    jmp $
    ret

; Get sector number
get_sector_number:
    xor ebx, ebx
    add ebx, 4
    mul ebx
    mov ebx, eax
    add ebx, 68
    ret