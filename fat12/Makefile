ISO12			:= build/fat12.img
ISO16			:= build/fat16.img
ACTIVE_ISO		:= $(ISO16)
.PHONY: all clean run debug gdb build mount umount

all: clean build run

build:
	nasm -f bin -o build/boot.bin src/bootloader.asm
	sed '2d' src/bootloader.asm > src/debug.asm
	nasm -f elf -g -o build/boot.o src/debug.asm
# nasm -f bin -o build/k.bin src/k.asm

# gcc -m32 -ffreestanding -nostdlib -fno-pie -c src/kernel.c -o build/kernel.o
# ld -T linker.ld -m elf_i386 -o build/kernel.bin build/kernel.o

# dd if=/dev/zero of=$(ISO12) bs=512 count=2880
# mkfs.fat -F 12 $(ISO12)

# sudo mount -o loop $(ISO12) /mnt

# sudo cp src/one.txt /mnt
# sudo cp src/two.txt /mnt
# sudo cp src/three.txt /mnt
# sudo cp src/four.txt /mnt

# sudo umount /mnt

	dd if=/dev/zero of=$(ISO16) bs=1M count=10
	mkfs.fat -F 16 $(ISO16)
	
	dd if=build/boot.bin of=$(ISO16) conv=notrunc

	sudo mount -o loop $(ISO16) /mnt

	sudo cp src/one.txt /mnt
	sudo cp src/two.txt /mnt
	sudo cp src/three.txt /mnt
	sudo cp src/four.txt /mnt

	sudo umount /mnt

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
	qemu-system-i386 -hda $(ACTIVE_ISO)

debug: clean build
	qemu-system-i386 -hda $(ACTIVE_ISO) -s -S

gdb:
	gdb -ex "file build/boot.o" -ex "target remote localhost:1234" -ex "layout asm" -ex "b load_cluster" -ex "c"

