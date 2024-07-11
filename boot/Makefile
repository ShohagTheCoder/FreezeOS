# Variables
BUILD_BOOT_DIR		:= ../build/boot
BOOT_ASM			:= boot.asm
BOOT_DEBUG_ASM		:= boot-debug.asm
BOOT_BIN			:= $(BUILD_BOOT_DIR)/boot.bin
BOOT_OBJ			:= $(BUILD_BOOT_DIR)/boot.o

# Rulues
.PHONY: all
all: $(BOOT_BIN)

$(BOOT_BIN) : $(BOOT_ASM)
	@mkdir -p $(@D)
	nasm -f bin $< -o $@
	sed '3d' $< > $(BOOT_DEBUG_ASM)
	nasm -f elf -g $(BOOT_DEBUG_ASM) -o $(BOOT_OBJ)