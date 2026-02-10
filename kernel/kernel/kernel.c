/**
 * @file kernel.c
 * @brief Main kernel entry point.
 *
 * This file contains the main kernel entry point (kernel_main) which is
 * called by the bootloader after the system has been initialized.
 */

#include <kernel/tty.h>

#include "kernel/multiboot.h"
#include "kernel/types.h"

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This kernel will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/**
 * @brief Kernel main entry point.
 *
 * This is the entry point called by the bootloader. It initializes
 * the terminal and displays a simple animation demonstrating the
 * VGA text mode capabilities.
 */
void kernel_main(const uint32_t magic_number, const multiboot_info_t* mbi)
{
    /* Initialize terminal interface */
    terminal_initialize();

    /* Display welcome message */
    terminal_writestring("Welcome to SoerensOS!\n");
    terminal_writestring("=====================\n\n");

    if (magic_number != MULTIBOOT_BOOTLOADER_MAGIC)
    {
        terminal_writestring("Invalid magic number: 0x\n");
        return;
    }

    if (mbi->flags & MULTIBOOT_INFO_FRAMEBUFFER)
    {
        // TODO: implement VBE framebuffer driver
        // TODO: think about a terminal abstraction layer that works with both vga & vbe
        terminal_writestring("Framebuffer detected (VBE mode)");
    }
    else
    {
        terminal_writestring("No framebuffer detected, using VGA text mode");
    }
}
