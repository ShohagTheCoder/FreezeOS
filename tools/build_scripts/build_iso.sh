#!/bin/bash

set -e  # Exit immediately if a command fails

# Clean previous build artifacts
rm -rf build
mkdir -p build/boot

# Assemble and link the bootloader
nasm -f bin bootloader/custom/bootloader.asm -o build/bootloader.bin

# Compile the kernel
gcc -ffreestanding -c kernel/kernel.c -o build/kernel.o
ld -Ttext 0x1000 --oformat binary build/kernel.o -o build/kernel.bin

# Create bootable disk image
dd if=/dev/zero of=build/disk.img bs=512 count=2880  # Create 1.44MB disk image (floppy)
dd if=build/bootloader.bin of=build/disk.img conv=notrunc bs=512 seek=0

# Copy kernel to disk image
dd if=build/kernel.bin of=build/disk.img conv=notrunc bs=512 seek=1

# Create ISO file
mkdir -p build/iso/boot/grub
cp build/disk.img build/iso/boot/
echo 'set timeout=0' > build/iso/boot/grub/grub.cfg
echo 'set default=0' >> build/iso/boot/grub/grub.cfg
echo 'menuentry "Custom OS" {' >> build/iso/boot/grub/grub.cfg
echo '    multiboot /boot/disk.img' >> build/iso/boot/grub/grub.cfg
echo '    boot' >> build/iso/boot/grub/grub.cfg
echo '}' >> build/iso/boot/grub/grub.cfg

grub-mkrescue -o os_project/os.iso build/iso

echo "ISO file created: os_project/os.iso"
