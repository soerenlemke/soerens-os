# ==============================================================================
# SoerensOS Makefile - Meaty Skeleton Structure
# ==============================================================================

# Cross-compiler settings
PREFIX  ?= $(HOME)/opt/cross
TARGET  ?= i686-elf
ARCH    := i386

CC      := $(PREFIX)/bin/$(TARGET)-gcc
AS      := $(PREFIX)/bin/$(TARGET)-as
AR      := $(PREFIX)/bin/$(TARGET)-ar

# Directories
BUILD     := build
SYSROOT   := $(BUILD)/sysroot
ISO_DIR   := isodir

# Output files
KERNEL    := $(BUILD)/soerens-os.kernel
ISO       := soerens-os.iso

# Compiler flags
CFLAGS    := -std=gnu99 -ffreestanding -O2 -Wall -Wextra -g
CPPFLAGS  := -D__is_kernel -Ikernel/include
LDFLAGS   := -ffreestanding -O2 -nostdlib
LIBS      := -lgcc

# ==============================================================================
# Libc
# ==============================================================================

LIBC_CFLAGS   := $(CFLAGS) -Ilibc/include
LIBC_CPPFLAGS := -D__is_libc

LIBC_OBJS := \
	$(BUILD)/libc/string/strlen.o \
	$(BUILD)/libc/string/memcmp.o \
	$(BUILD)/libc/string/memcpy.o \
	$(BUILD)/libc/string/memmove.o \
	$(BUILD)/libc/string/memset.o

LIBC := $(BUILD)/libc.a

# ==============================================================================
# Kernel
# ==============================================================================

KERNEL_CFLAGS   := $(CFLAGS) -Ilibc/include -Ikernel/include
KERNEL_CPPFLAGS := -D__is_kernel
KERNEL_LDFLAGS  := -T kernel/arch/$(ARCH)/linker.ld $(LDFLAGS)

KERNEL_OBJS := \
	$(BUILD)/kernel/arch/$(ARCH)/boot.o \
	$(BUILD)/kernel/arch/$(ARCH)/tty.o \
	$(BUILD)/kernel/kernel/kernel.o

# ==============================================================================
# Build Rules
# ==============================================================================

.PHONY: all clean iso run-kernel run-iso

all: $(KERNEL)

# Create build directories
$(BUILD):
	mkdir -p $(BUILD)

$(BUILD)/libc/string:
	mkdir -p $(BUILD)/libc/string

$(BUILD)/kernel/arch/$(ARCH):
	mkdir -p $(BUILD)/kernel/arch/$(ARCH)

$(BUILD)/kernel/kernel:
	mkdir -p $(BUILD)/kernel/kernel

# ------------------------------------------------------------------------------
# Libc build rules
# ------------------------------------------------------------------------------

$(BUILD)/libc/string/%.o: libc/string/%.c | $(BUILD)/libc/string
	$(CC) $(LIBC_CFLAGS) $(LIBC_CPPFLAGS) -c $< -o $@

$(LIBC): $(LIBC_OBJS)
	$(AR) rcs $@ $(LIBC_OBJS)

# ------------------------------------------------------------------------------
# Kernel build rules
# ------------------------------------------------------------------------------

$(BUILD)/kernel/arch/$(ARCH)/boot.o: kernel/arch/$(ARCH)/boot.s | $(BUILD)/kernel/arch/$(ARCH)
	$(AS) $< -o $@

$(BUILD)/kernel/arch/$(ARCH)/tty.o: kernel/arch/$(ARCH)/tty.c | $(BUILD)/kernel/arch/$(ARCH)
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) -c $< -o $@

$(BUILD)/kernel/kernel/kernel.o: kernel/kernel/kernel.c | $(BUILD)/kernel/kernel
	$(CC) $(KERNEL_CFLAGS) $(KERNEL_CPPFLAGS) -c $< -o $@

$(KERNEL): $(KERNEL_OBJS) $(LIBC) kernel/arch/$(ARCH)/linker.ld
	$(CC) $(KERNEL_LDFLAGS) $(KERNEL_OBJS) $(LIBC) $(LIBS) -o $@

# ------------------------------------------------------------------------------
# ISO build (requires Docker on macOS)
# ------------------------------------------------------------------------------

$(ISO_DIR)/boot/soerens-os: $(KERNEL)
	mkdir -p $(ISO_DIR)/boot
	cp $(KERNEL) $(ISO_DIR)/boot/soerens-os

# Build ISO using Docker (recommended for macOS)
$(ISO): $(ISO_DIR)/boot/soerens-os $(ISO_DIR)/boot/grub/grub.cfg
	@echo "Building ISO with Docker..."
	docker run --platform linux/amd64 --rm \
		-v "$(shell pwd)":/src \
		-w /src \
		soerens-os-builder \
		grub-mkrescue -o $(ISO) $(ISO_DIR)

iso: $(ISO)

# ------------------------------------------------------------------------------
# Run targets
# ------------------------------------------------------------------------------

# Run with ISO (required for framebuffer support via GRUB)
run: $(ISO)
	qemu-system-i386 -cdrom $(ISO)

# Quick kernel test without framebuffer (VGA text mode, no GRUB)
run-text: $(KERNEL)
	qemu-system-i386 -kernel $(KERNEL)

# ------------------------------------------------------------------------------
# Docker (one-time setup for grub-mkrescue on macOS)
# ------------------------------------------------------------------------------

DOCKER_IMAGE := soerens-os-builder

.PHONY: docker-build

# Build the Docker image (one-time) - force amd64 for grub-pc-bin
docker-build:
	docker build --platform linux/amd64 -t $(DOCKER_IMAGE) .

# ------------------------------------------------------------------------------
# Clean
# ------------------------------------------------------------------------------

clean:
	rm -rf $(BUILD)
	rm -f $(ISO)
	rm -f $(ISO_DIR)/boot/soerens-os
