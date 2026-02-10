/**
 * @file tty.h
 * @brief Terminal (TTY) interface for the kernel.
 *
 * This header provides the interface for terminal operations.
 * The actual implementation is architecture-specific.
 */

#ifndef _KERNEL_TTY_H
#define _KERNEL_TTY_H

#include <stddef.h>
#include "types.h"

/**
 * @brief VGA color palette constants.
 *
 * These constants represent the 16 colors available in VGA text mode.
 */
enum vga_color
{
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

/**
 * @brief Creates a VGA color byte from foreground and background colors.
 *
 * @param fg Foreground color.
 * @param bg Background color.
 * @return Combined color byte.
 */
static uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

/**
 * @brief Initializes the terminal.
 *
 * Sets up the terminal for output by clearing the screen and
 * setting default colors.
 */
void terminal_initialize(void);

/**
 * @brief Sets the terminal text color.
 *
 * @param color The color byte (use vga_entry_color to create).
 */
void terminal_setcolor(uint8_t color);

/**
 * @brief Outputs a single character to the terminal.
 *
 * @param c The character to output.
 */
void terminal_putchar(char c);

/**
 * @brief Writes a buffer of characters to the terminal.
 *
 * @param data Pointer to the character buffer.
 * @param size Number of characters to write.
 */
void terminal_write(const char* data, size_t size);

/**
 * @brief Writes a null-terminated string to the terminal.
 *
 * @param data Pointer to the null-terminated string.
 */
void terminal_writestring(const char* data);

#endif /* _KERNEL_TTY_H */
