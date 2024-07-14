[BITS 32]
[org 0xa000]

kernel_loader:
    mov byte [0xb8000], 'L'
    mov byte [0xb8001], 0x4f

    jmp $