.PHONY: all clean run debug gdb build

all: clean build run

build:
	nasm -f bin -o build/boot.bin src/bootloader.asm
	nasm -f bin -o build/k.bin src/k.asm

	gcc -m32 -ffreestanding -nostdlib -fno-pie -c src/kernel.c -o build/kernel.o
	ld -T linker.ld -m elf_i386 -o build/kernel.bin build/kernel.o

	dd if=/dev/zero of=build/fat12.img bs=512 count=2880
	mkfs.fat -F 12 -n "FreezeOS" build/fat12.img

	dd if=build/boot.bin of=build/fat12.img conv=notrunc

	sudo mount -o loop build/fat12.img /mnt
# sudo cp build/kernel.bin /mnt
	sudo cp src/one.txt /mnt
	sudo cp src/two.txt /mnt
# sudo cp build/k.bin /mnt
	sudo umount /mnt

clean:
	rm -rf build/*

run:
	qemu-system-i386 -hda build/fat12.img

debug:
	qemu-system-i386 -hda build/fat12.img -s -S

gdb:
	gdb -ex "target remote localhost:1234" -ex "layout asm" -ex "b *0x7c00" -ex "c"

