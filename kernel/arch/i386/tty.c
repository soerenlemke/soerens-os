/**
 * @file tty.c
 * @brief VGA Text Mode Terminal Driver for i386 architecture.
 *
 * This file implements a text-mode terminal driver for the VGA hardware
 * typically found on x86 systems. It provides functions for initializing
 * the terminal, writing characters and strings, and managing cursor position.
 */

#include <kernel/tty.h>
#include <stdint.h>
#include <string.h>

/* ========================================================================
 * Hardware Constants
 * ======================================================================== */

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000

/* ========================================================================
 * Private Variables
 * ======================================================================== */

static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

/* ========================================================================
 * Private Function Declarations
 * ======================================================================== */

/**
 * @brief Creates a VGA text mode entry with character and color attributes.
 *
 * @param uc The character to display.
 * @param color The color attribute (foreground and background color).
 *
 * @return A 16-bit value combining the character in the lower byte and color in the
 * upper byte.
 */
static inline uint16_t vga_entry(unsigned char uc, uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

/**
 * @brief Scrolls the terminal display up by one line.
 *
 * When the terminal reaches the bottom of the display, this function
 * shifts all existing content up by one line, clearing the bottom line
 * and resetting the cursor to the start of the new bottom line.
 */
static void terminal_scroll(void)
{
    for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            size_t from_index = (y + 1) * VGA_WIDTH + x;
            size_t to_index = y * VGA_WIDTH + x;
            terminal_buffer[to_index] = terminal_buffer[from_index];
        }
    }
    for (size_t x = 0; x < VGA_WIDTH; x++)
    {
        size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
        terminal_buffer[index] = vga_entry(' ', terminal_color);
    }
    terminal_row = VGA_HEIGHT - 1;
    terminal_column = 0;
}

/**
 * @brief Writes a character entry to the VGA terminal buffer at the specified coordinates.
 *
 * @param c The character to write.
 * @param color The color attribute to use.
 * @param x The x-coordinate (column).
 * @param y The y-coordinate (row).
 */
static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y)
{
    size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

/* ========================================================================
 * Public Function Implementations
 * ======================================================================== */

void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)VGA_MEMORY;

    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

void terminal_setcolor(uint8_t color)
{
    terminal_color = color;
}

void terminal_putchar(char c)
{
    if (c == '\n')
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_scroll();
        }
        return;
    }
    if (c == '\r')
    {
        terminal_column = 0;
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT)
        {
            terminal_scroll();
        }
    }
}

void terminal_write(const char* data, size_t size)
{
    for (size_t i = 0; i < size; i++)
    {
        terminal_putchar(data[i]);
    }
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}
