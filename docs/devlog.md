# Development Log

## 2026-01-31 - 2026-02-04

- Started following the Bare Bones tutorial
- Implemented VGA text mode
- Added an ASCII scene for testing
- Created a simple Makefile for building the kernel and creating a bootable image
- Tested the kernel in QEMU, it successfully displayed the ASCII scene
- Added support for newline characters to the terminal driver
- implemented terminal scrolling when the text exceeds the screen height

## 2026-02-05 - 2026-02-06

- implemented rendering colorful ASCII art with example in kernel main function
- GitHub pages deployment for documentation
- refactoring the codebase to minimize public API in vga module

## 2026-02-07

- build project on macOS
- added documentation for setting up cross-compilation toolchain on macOS

## 2026-02-08

- cleanup & refactor
- add new functions to public API of vga module
- add new functions for setting the cursor position, clearing the screen and public api newline
- add docker support for building the project in a consistent environment across different host OSes

## 2026-02-09

- added documentation for differences between QEMU `-kernel` mode and booting from ISO with GRUB
- running make run now boots the ISO with GRUB, allowing for VBE framebuffer support in the future
- running make run-text still uses `-kernel` mode for quick testing without GRUB, but without VBE support