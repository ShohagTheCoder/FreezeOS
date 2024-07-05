# Variables
BUILD_DIR			:= build
ISO_DIR				:= $(BUILD_DIR)/iso
ISO_FILE			:= $(ISO_DIR)/freeze.img
BOOT_BIN		:= $(BUILD_DIR)/boot/boot.bin
KERNEL_BIN			:= $(BUILD_DIR)/kernel/kernel.bin
KERNEL_ELF			:= $(BUILD_DIR)/kernel/kernel.elf


# Rules
.PHONY : all clean run debug bootloader kernel
all : clean $(ISO_FILE) run

$(ISO_FILE): kernel
	@mkdir -p $(@D)
	dd if=/dev/zero of=$(ISO_FILE) bs=512 count=2880
	dd if=$(BOOT_BIN) of=$@ conv=notrunc
	dd if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc

kernel:
	$(MAKE) -C boot
	$(MAKE) -C kernel

run : 
	qemu-system-i386 -drive format=raw,file=$(ISO_FILE)

clean:
	rm -rf $(BUILD_DIR)/*
	clear

# Debug section
debug: 
	qemu-system-i386 -s -S -kernel $(ISO_FILE)

gdb:
	gdb  -ex "file $(KERNEL_ELF)" -ex "target remote localhost:1234" -ex "layout asm"
