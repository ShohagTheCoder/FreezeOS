# Variables
SRC				:= src
DIST			:= dist
BUILD			:= build
QEMU			:= qemu-system-x86_64
CROSS_COMPILE 	:= i686-linux-gnu-
GCC 			:= $(CROSS_COMPILE)gcc
LD 				:= $(CROSS_COMPILE)ld
EDITOR			:= micro
ISO				:= $(BUILD)/freeze.img
ASM				:= nasm

# Files
KERNEL_OBJ		:= $(DIST)/kernel/kernel.o
KERNEL_C		:= $(SRC)/kernel/kernel.c
KERNEL_BIN		:= $(DIST)/kernel/kernel.bin

BOOTLOADER_BIN	:= $(DIST)/bootloader/bootloader.bin
BOOTLOADER_ASM	:= $(SRC)/bootloader/bootloader.asm

SCREEN_OBJ		:= $(DIST)/kernel/functions/screen.o
SRCEEN_C		:= $(SRC)/kernel/functions/screen.c

# All rules
all: clean build-boot compile-kernel compile-functions link-kernel-with-functions build-img run

# Only kernel
kernel: clean build-boot compile-kernel link-kernel build-img run

# Rules
build-boot: $(BOOTLOADER_ASM)
	$(ASM) -f bin $(BOOTLOADER_ASM) -o $(BOOTLOADER_BIN)

# Build the image file if not exist
create-img:
	dd if=/dev/zero of=$(ISO) bs=512 count=2880;

# Copy data from bootloader.bin to freeze.img
build-img: $(BOOTLOADER_BIN)
	dd if=$(BOOTLOADER_BIN) of=$(ISO) conv=notrunc
	dd if=$(KERNEL_BIN) of=$(ISO) bs=512 seek=1 conv=notrunc

run: $(ISO)
	$(QEMU) -drive format=raw,file=$(ISO)

# Edit the bootlaoder
edit: $(BOOTLOADER_ASM)
	$(EDITOR) $(BOOTLOADER_ASM)

# Edit the make file
make:
	$(EDITOR) Makefile

compile-kernel: $(KERNEL_C)
	$(GCC) -ffreestanding -fno-pie -nostdlib -c $(KERNEL_C) -o $(KERNEL_OBJ) 

compile-functions:
	$(GCC) -ffreestanding -fno-pie -nostdlib  -c $(SRCEEN_C) -o $(SCREEN_OBJ)

# Linkt the kernel
link-kernel: $(KERNEL_OBJ)
	$(LD) -T linker.ld -o $(KERNEL_BIN) $(KERNEL_OBJ)

# Linkt the kernel
link-kernel-with-functions: $(KERNEL_OBJ)
	$(LD) -T linker.ld -o $(KERNEL_BIN) $(KERNEL_OBJ) $(SCREEN_OBJ)

# Clear kernel
clean:
	rm -f $(KERNEL_BIN) $(KERNEL_OBJ) $(BOOTLOADER_BIN) $(SCREEN_OBJ)
	
format:
	clang-format -style=file -i kernel/
