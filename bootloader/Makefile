# Variables
BUILD_BOOT_DIR		:= ../build/bootloader
BOOTLOADER_ASM		:= boot.asm
BOOTLOADER_BIN		:= $(BUILD_BOOT_DIR)/boot.bin

# Rulues
.PHONY: all
all: $(BOOTLOADER_BIN)

$(BOOTLOADER_BIN) : $(BOOTLOADER_ASM)
	@mkdir -p $(@D)
	nasm -f bin $< -o $@