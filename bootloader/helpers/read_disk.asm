; Read from disk
read_disk:
    mov ah, 0x02 		; BIOS read sector function
    mov ch, 0
	mov dh, 0
    int 0x13
    ret