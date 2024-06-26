# Makefile for building custom OS with custom bootloader

# Output directories
BUILD_DIR 			:= ../../build
ISO_DIR 			:= $(BUILD_DIR)/iso
BOOT_DIR 			:= $(ISO_DIR)/boot

# Filenames
BOOTLOADER_SRC 		:= ../bootloader/bootloader.asm
KERNEL_SRC 			:= ../kernel/kernel.c
BOOTLOADER_BIN		:= $(BUILD_DIR)/bootloader.bin
KERNEL_BIN			:= $(BUILD_DIR)/kernel.bin
DISK_IMG 			:= $(BUILD_DIR)/disk.img
ISO_FILE 			:= ../../build/os.iso

# Toolchain
ASM 				:= nasm
CC 					:= gcc
LD 					:= ld
DD 					:= dd
GRUB_MKRESCUE 		:= grub-mkrescue

# Compilation flags
ASMFLAGS 			:= -f bin
CFLAGS 				:= -ffreestanding -m32

# Targets
.PHONY: all clean

all: $(ISO_FILE)

$(ISO_FILE): $(DISK_IMG)
    mkdir -p $(BOOT_DIR)
    cp $(DISK_IMG) $(BOOT_DIR)/
    grub-mkrescue -o $(ISO_FILE) $(ISO_DIR)

$(DISK_IMG): $(BOOTLOADER_BIN) $(KERNEL_BIN)
    $(DD) if=/dev/zero of=$(DISK_IMG) bs=512 count=2880  # Create 1.44MB disk image (floppy)
    $(DD) if=$(BOOTLOADER_BIN) of=$(DISK_IMG) conv=notrunc bs=512 seek=0

$(BOOTLOADER_BIN): $(BOOTLOADER_SRC)
    $(ASM) $(ASMFLAGS) $< -o $@

$(KERNEL_BIN): $(KERNEL_SRC)
    $(CC) $(CFLAGS) -c $< -o $(BUILD_DIR)/kernel.o
    $(LD) -Ttext 0x1000 --oformat binary $(BUILD_DIR)/kernel.o -o $@

clean:
    # rm -rf $(BUILD_DIR)
    # rm -f $(ISO_FILE)
