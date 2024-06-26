BUILD_DIR			:= build
SRC_KERNEL_DIR		:= kernel
SRC_BOOT_DIR		:= bootloader/custom

ISO_DIR				:= $(BUILD_DIR)/iso
ISO_FILE			:= $(ISO_DIR)/freeze.img

BUILD_BOOT_DIR		:= $(BUILD_DIR)/bootloader
BUILD_KERNEL_DIR	:= $(BUILD_DIR)/kernel

# Source files
BOOTLOADER_ASM		:= $(SRC_BOOT_DIR)/bootloader.asm
X86_START_ASM		:= $(SRC_KERNEL_DIR)/arch/x86/start.asm
KERNEL_C			:= $(SRC_KERNEL_DIR)/kernel.c
LD_FILE				:= configs/kernel/linker.ld

# Object files
BOOTLOADER_OBJ		:= $(BUILD_BOOT_DIR)/bootloader.o
X86_START_OBJ		:= $(BUILD_KERNEL_DIR)/start.o
KERNEL_OBJ			:= $(BUILD_KERNEL_DIR)/kernel.o

# Binary files
BOOTLOADER_BIN		:= $(BUILD_BOOT_DIR)/bootlaoder.bin
KERNEL_BIN			:= $(BUILD_KERNEL_DIR)/kernel.bin

BUILD_BIN_FILES		:= $(shell find $(BUILD_DIR) -name '*.bin')
BUILD_OBJ_FILES		:= $(shell find $(BUILD_DIR) -name '*.o')

# Toolchain
ASM					:= nasm
CC					:= i686-linux-gnu-gcc
LD					:= i686-linux-gnu-ld
QEMU				:= qemu-system-x86_64
DD 					:= dd

KERNEL_C_FILES		:= $(shell find $(SRC_KERNEL_DIR) -name '*.c')
KERNEL_OBJ_FILES	:= $(patsubst $(SRC_KERNEL_DIR)/%.c, $(BUILD_KERNEL_DIR)/%.o, $(KERNEL_C_FILES))

# Compilation flags
ASMFLAGS			:= -f bin
CFLAGS				:= -ffreestanding -fno-pie -nostdlib

# All rules
.PHONY : all clean create-iso run

all : $(ISO_FILE) run

create-iso : 
	$(DD) if=/dev/zero of=$(ISO_FILE) bs=512 count=2880

$(ISO_FILE): $(BOOTLOADER_BIN) $(KERNEL_BIN)
	$(DD) if=$(BOOTLOADER_BIN) of=$@ bs=512 seek=0 conv=notrunc
	$(DD) if=$(KERNEL_BIN) of=$@ bs=512 seek=1 conv=notrunc


$(KERNEL_BIN) : $(KERNEL_OBJ_FILES)
	$(LD) -T $(LD_FILE) -o $@ $(KERNEL_OBJ_FILES)
	
$(BUILD_KERNEL_DIR)/%.o : $(SRC_KERNEL_DIR)/%.c
	@mkdir -p $(@D)
	$(CC) $(CFLAGS) -c $< -o $@

	
$(BOOTLOADER_BIN) : $(BOOTLOADER_ASM)
	$(ASM) $(ASMFLAGS) $< -o $@

run : 
	$(QEMU) -drive format=raw,file=$(ISO_FILE)

clean:
	rm $(BUILD_BIN_FILES) $(BUILD_OBJ_FILES)