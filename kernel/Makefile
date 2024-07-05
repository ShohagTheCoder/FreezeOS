# Variables
BUILD_KERNEL_DIR	:= ../build/kernel
KERNEL_C_FILES		:= kernel.c drivers/keyboard.c lib/string.c  drivers/console.c io.c
KERNEL_OBJ_FILES	:= $(patsubst %.c, $(BUILD_KERNEL_DIR)/%.o, $(KERNEL_C_FILES))
KERNEL_ELF	 		:= $(BUILD_KERNEL_DIR)/kernel.elf
KERNEL_BIN	 		:= $(BUILD_KERNEL_DIR)/kernel.bin

# Configurations
LD_FILE				:= ../configs/kernel/linker.ld

# Flags
CFLAGS				:= -ffreestanding -Wall -nostdlib -m32

# Rules
.PHONY: all
all: $(KERNEL_BIN)

$(KERNEL_BIN): $(KERNEL_OBJ_FILES)
	ld -T $(LD_FILE) -m elf_i386 -o $(KERNEL_ELF) $^
	objcopy -O binary $(KERNEL_ELF) $@ 
	
$(BUILD_KERNEL_DIR)/%.o: %.c
	@mkdir -p $(@D)
	gcc $(CFLAGS) -c $< -o $@