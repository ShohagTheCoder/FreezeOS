ISO12			:= build/fat12.img
ISO16			:= build/fat16.img
.PHONY: all clean run debug gdb build mount umount

all: clean build run

build:
	nasm -f bin -o build/boot.bin src/bootloader.asm
# nasm -f bin -o build/k.bin src/k.asm

# gcc -m32 -ffreestanding -nostdlib -fno-pie -c src/kernel.c -o build/kernel.o
# ld -T linker.ld -m elf_i386 -o build/kernel.bin build/kernel.o

	dd if=/dev/zero of=$(ISO12) bs=512 count=2880
	dd if=/dev/zero of=$(ISO16) bs=1M count=10

	mkfs.fat -F 12 $(ISO12)
	mkfs.fat -F 16 $(ISO16)

	sudo mount -o loop $(ISO12) /mnt

	sudo cp src/one.txt /mnt
	sudo cp src/two.txt /mnt
	sudo cp src/three.txt /mnt
	sudo cp src/four.txt /mnt

	sudo umount /mnt

	sudo mount -o loop $(ISO16) /mnt

	sudo cp src/one.txt /mnt
	sudo cp src/two.txt /mnt
	sudo cp src/three.txt /mnt
	sudo cp src/four.txt /mnt

	sudo umount /mnt

# dd if=build/boot.bin of=$(ISO) conv=notrunc
# sudo cp build/kernel.bin /mnt
# sudo cp build/k.bin /mnt

mount12:
	sudo mount -o loop $(ISO12) /mnt

mount16:
	sudo mount -o loop $(ISO16) /mnt

umount:
	sudo umount /mnt

clean:
	rm -rf build/*

run:
	qemu-system-i386 -hda $(ISO)

debug:
	qemu-system-i386 -hda $(ISO) -s -S

gdb:
	gdb -ex "target remote localhost:1234" -ex "layout asm" -ex "b *0x7c00" -ex "c"

