# Variables
BUILD_KERNEL_DIR	:= ../build/kernel
BUILD_BOOT_DIR		:= ../build/boot
BOOT_OBJ			:= $(BUILD_BOOT_DIR)/boot.o
KERNEL_C_FILES		:= $(shell cat kernels)
KERNEL_OBJ_FILES	:= $(patsubst %.c, $(BUILD_KERNEL_DIR)/%.o, $(KERNEL_C_FILES))
KERNEL_ELF	 		:= $(BUILD_KERNEL_DIR)/kernel.elf
DEBUG_ELF	 		:= $(BUILD_KERNEL_DIR)/debug.elf
KERNEL_BIN	 		:= $(BUILD_KERNEL_DIR)/kernel.bin
KERNEL_ENTRY_ASM	:= arch/x86/kernel_entry.asm
KERNEL_ENTRY_OBJ	:= $(BUILD_KERNEL_DIR)/kernel_entry.o

# Configurations
LD_FILE				:= ../configs/kernel/linker.ld

# Flags
CFLAGS				:= -ffreestanding -Wall -nostdlib -m32 -g

# Rules
.PHONY: all
all: $(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_ENTRY_OBJ) $(KERNEL_OBJ_FILES)
	ld -T $(LD_FILE) -m elf_i386 -o $(KERNEL_ELF) $^
	objcopy -O binary $(KERNEL_ELF) $@
	
$(BUILD_KERNEL_DIR)/%.o: %.c
	@mkdir -p $(@D)
	gcc $(CFLAGS) -c $< -o $@

$(KERNEL_ENTRY_OBJ): $(KERNEL_ENTRY_ASM)
	@mkdir -p $(@D)
	nasm -f elf32 -g -o $@ $<