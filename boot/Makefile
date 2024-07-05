# Variables
BUILD_BOOT_DIR		:= ../build/boot
BOOT_ASM			:= boot.asm
BOOT_BIN			:= $(BUILD_BOOT_DIR)/boot.bin

# Rulues
.PHONY: all
all: $(BOOT_BIN)

$(BOOT_BIN) : $(BOOT_ASM)
	@mkdir -p $(@D)
	nasm -f bin $< -o $@