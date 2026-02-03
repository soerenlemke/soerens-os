PREFIX ?= $(HOME)/opt/cross
TARGET ?= i686-elf

CC := $(PREFIX)/bin/$(TARGET)-gcc
AS := $(PREFIX)/bin/$(TARGET)-as

# Include paths for headers
INCLUDES := -I src/drivers -I src/lib

CFLAGS  := -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(INCLUDES)
LDFLAGS := -T linker.ld -ffreestanding -O2 -nostdlib

BUILD   := build
ISO_DIR := isodir
KERNEL  := $(BUILD)/myos.elf
ISO     := myos.iso

OBJS := $(BUILD)/boot.o \
        $(BUILD)/kernel.o \
        $(BUILD)/vga.o \
        $(BUILD)/string.o

.PHONY: all clean iso run-iso run-kernel

all: $(KERNEL)

# --- build dir ---
$(BUILD):
	mkdir -p $(BUILD)

# --- compile ---
$(BUILD)/boot.o: src/boot.s | $(BUILD)
	$(AS) $< -o $@

$(BUILD)/kernel.o: src/kernel/kernel.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/vga.o: src/drivers/vga.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD)/string.o: src/lib/string.c | $(BUILD)
	$(CC) $(CFLAGS) -c $< -o $@

# --- link ---
$(KERNEL): $(OBJS) linker.ld
	$(CC) $(LDFLAGS) $(OBJS) -o $@ -lgcc

# --- stage kernel for ISO (GRUB expects /boot/myos) ---
$(ISO_DIR)/boot/myos: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL) $(ISO_DIR)/boot/myos

# --- ISO ---
$(ISO): $(ISO_DIR)/boot/myos $(ISO_DIR)/boot/grub/grub.cfg
	grub-mkrescue -o $(ISO) $(ISO_DIR)

iso: $(ISO)

# --- run ---
run-kernel: $(KERNEL)
	env -u LD_LIBRARY_PATH -u LD_PRELOAD qemu-system-i386 -kernel $(KERNEL)

run-iso: $(ISO)
	env -u LD_LIBRARY_PATH -u LD_PRELOAD qemu-system-i386 -cdrom $(ISO)

clean:
	rm -rf $(BUILD)
	rm -f $(ISO)
	rm -f $(ISO_DIR)/boot/myos
