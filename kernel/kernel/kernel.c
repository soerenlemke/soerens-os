/**
 * @file kernel.c
 * @brief Main kernel entry point.
 *
 * This file contains the main kernel entry point (kernel_main) which is
 * called by the bootloader after the system has been initialized.
 */

#include <stdbool.h>
#include <stdint.h>

#include <kernel/tty.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This kernel will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This kernel needs to be compiled with a ix86-elf compiler"
#endif

/**
 * @brief Delay loop for visible animation.
 *
 * This is a simple busy-wait delay function used to slow down
 * animations so they are visible to the user.
 *
 * @param count Number of iterations to delay.
 */
static void delay(uint32_t count)
{
    for (volatile uint32_t i = 0; i < count; i++)
    {
        /* volatile prevents compiler optimization */
    }
}

/**
 * @brief Kernel main entry point.
 *
 * This is the entry point called by the bootloader. It initializes
 * the terminal and displays a simple animation demonstrating the
 * VGA text mode capabilities.
 */
void kernel_main(void)
{
    /* Initialize terminal interface */
    terminal_initialize();

    /* Display welcome message */
    terminal_writestring("Welcome to SoerensOS!\n");
    terminal_writestring("=====================\n\n");

    uint8_t color_index = 0;
    const uint8_t num_colors = 16;
    const uint32_t delay_count = 50000000;

    while (true)
    {
        const uint8_t bg_color = color_index % num_colors;
        const uint8_t fg_color = (color_index + 8) % num_colors;

        terminal_setcolor(vga_entry_color(fg_color, bg_color));
        terminal_writestring("   |    o\\\n");
        terminal_writestring("|==|__    O\n");
        terminal_writestring("|  'WW    /\\\n");
        terminal_writestring("           /\\__\n");
        terminal_writestring("           \\\n");
        delay(delay_count);
        terminal_initialize();

        terminal_setcolor(vga_entry_color(bg_color, fg_color));
        terminal_writestring("   | o\\\n");
        terminal_writestring("|==|_  \\O/\n");
        terminal_writestring("|  'WW  |\n");
        terminal_writestring("         /\\\n");
        terminal_writestring("        /  \\\n");
        delay(delay_count);
        terminal_initialize();

        terminal_setcolor(vga_entry_color(fg_color, bg_color));
        terminal_writestring("   |\n");
        terminal_writestring("|==O\n");
        terminal_writestring("|  |WW \\o/\n");
        terminal_writestring("        |\n");
        terminal_writestring("       / \\\n");
        delay(delay_count);
        terminal_initialize();

        color_index++;
    }
}
