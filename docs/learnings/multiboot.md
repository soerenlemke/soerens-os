# Multiboot Header Implementation

## Overview

Multiboot specification enables bootloaders (GRUB) to load our kernel and pass boot information like memory maps and
framebuffer details.

## Header Structure

- **Location**: First 8192 bytes of kernel binary in `.multiboot` section
- **Magic Number**: `0x1BADB002` for bootloader detection
- **Flags**: Define required features (module alignment, memory info, video mode)
- **Checksum**: `-(MAGIC + FLAGS)` ensures header integrity

## Boot Information Access

Bootloader passes `multiboot_info_t*` pointer in `ebx` register containing:

- Memory map, boot device, command line
- **Framebuffer information** (`MULTIBOOT_INFO_FRAMEBUFFER` flag) for VGA vs VBE detection

Pointer preserved in `boot.S` and passed to `kernel_main(uint32_t magic, multiboot_info_t* mbi)`.
