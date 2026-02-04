//
// Created by soerenlemke on 01.02.26.
//

#include "vga.h"
#include "string.h"

/* ========================================================================
 * Private Variables
 * ======================================================================== */

size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

/* ========================================================================
 * Private Function Declarations
 * ======================================================================== */

/**
 * scroll_terminal - Scrolls the terminal display up by one line
 *
 * When the terminal reaches the bottom of the display, this function
 * shifts all existing content up by one line, clearing the bottom line
 * and resetting the cursor to the start of the new bottom line.
 *
 * Returns: void
 */
static void scroll_terminal(void)
{
    terminal_column = 0;
    if (++terminal_row == VGA_HEIGHT)
    {
        for (size_t y = 0; y < VGA_HEIGHT - 1; y++)
        {
            for (size_t x = 0; x < VGA_WIDTH; x++)
            {
                const size_t from_index = (y + 1) * VGA_WIDTH + x;
                const size_t to_index = y * VGA_WIDTH + x;
                terminal_buffer[to_index] = terminal_buffer[from_index];
            }
        }
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = (VGA_HEIGHT - 1) * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
        terminal_row = VGA_HEIGHT - 1;
    }
}

/**
 * @brief Writes a character entry to the VGA terminal buffer at the specified coordinates.
 *
 * Places a character with the given color attribute at position (x, y) in the VGA terminal.
 * If the coordinates exceed the terminal dimensions, they are clamped to the maximum valid values.
 *
 * @param c The character to write to the terminal buffer.
 * @param color The color attribute for the character (foreground and background colors).
 * @param x The x-coordinate (column) where the character should be written.
 * @param y The y-coordinate (row) where the character should be written.
 *
 * @note Coordinates are automatically clamped to valid ranges:
 *       x is clamped to [0, VGA_WIDTH - 1]
 *       y is clamped to [0, VGA_HEIGHT - 1]
 */
static void terminal_putentryat(const char c, const uint8_t color, size_t x, size_t y)
{
    size_t clamped_x = x;
    size_t clamped_y = y;

    if (clamped_x >= VGA_WIDTH)
    {
        clamped_x = VGA_WIDTH - 1;
    }

    if (clamped_y >= VGA_HEIGHT)
    {
        clamped_y = VGA_HEIGHT - 1;
    }

    const size_t index = clamped_y * VGA_WIDTH + clamped_x;
    terminal_buffer[index] = vga_entry(c, color);
}

/* ========================================================================
 * Public Function Declarations
 * ======================================================================== */

/**
 * @brief Combines foreground and background VGA colors into a single color byte.
 *
 * @param fg The foreground color (lower 4 bits).
 * @param bg The background color (upper 4 bits).
 *
 * @return A single byte where the lower 4 bits represent the foreground color
 *         and the upper 4 bits represent the background color.
 */
uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

/**
 * @brief Creates a VGA text mode entry with character and color attributes.
 *
 * @param uc The character to display.
 * @param color The color attribute (foreground and background color).
 *
 * @return A 16-bit value combining the character in the lower byte and color in the
 * upper byte.
 */
uint16_t vga_entry(const unsigned char uc, const uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

/**
 * @brief Initializes the VGA terminal.
 *
 * Sets up the terminal by resetting the cursor position to (0, 0),
 * setting the default color to light grey text on a black background,
 * and clearing the entire terminal buffer by filling it with blank
 * characters in the default color.
 *
 * @return void
 */
void terminal_initialize(void)
{
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);

    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }
}

/**
 * Sets the current terminal color for subsequent text output.
 *
 * @param color The color value to set. Should be a combination of foreground
 *              and background colors as defined in the VGA color palette.
 *
 * @return void
 */
void terminal_setcolor(const uint8_t color)
{
    terminal_color = color;
}

/**
 * @brief Outputs a single character to the terminal display.
 *
 * Handles special characters (newline and carriage return) and writes
 * regular characters to the current cursor position. Automatically scrolls
 * the terminal when the end of a line is reached.
 *
 * @param c The character to output to the terminal.
 *
 * @details
 * - '\n' (newline): Scrolls the terminal down.
 * - '\r' (carriage return): Resets the column to the start of the line.
 * - Other characters: Writes the character at the current cursor position
 *   using the current terminal color. If the column reaches VGA_WIDTH,
 *   the terminal is scrolled and the cursor wraps to the next line.
 */
void terminal_putchar(const char c)
{
    if (c == '\n')
    {
        scroll_terminal();
        return;
    }
    if (c == '\r') // carriage return
    {
        terminal_column = 0;
        return;
    }

    terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
    if (++terminal_column == VGA_WIDTH)
    {
        scroll_terminal();
    }
}

/**
 * terminal_write - Write a string of characters to the terminal
 * @data: Pointer to the character buffer to write
 * @size: Number of characters to write from the buffer
 *
 * Writes the specified number of characters from the given buffer to the
 * terminal by calling terminal_putchar() for each character.
 */
void terminal_write(const char* data, const size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

/**
 * @brief Writes a null-terminated string to the terminal.
 *
 * Outputs the string pointed to by @p data to the terminal display.
 * The length of the string is determined using strlen().
 *
 * @param data Pointer to a null-terminated string to write to the terminal.
 *
 * @note The string must be null-terminated.
 */
void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}

/**
 * terminal_draw_ascii_dunk_scene_one - Draws a ASCII art dunk scene to the terminal
 *
 * Outputs a simple ASCII art representation of a basketball dunk scene,
 * including a figure performing a dunk with a basketball hoop.
 *
 * Returns: void
 */
void terminal_draw_ascii_dunk_scene_one(void)
{
    terminal_writestring("   |    o\\\n");
    terminal_writestring("|==|__    O\n");
    terminal_writestring("|  'WW    /\\\n");
    terminal_writestring("           /\\__\n");
    terminal_writestring("           \\\n");
}

/**
 * terminal_draw_ascii_dunk_scene_two - Draws a ASCII art dunk scene to the terminal
 *
 * Outputs a simple ASCII art representation of a basketball dunk scene,
 * including a figure performing a dunk with a basketball hoop.
 *
 * Returns: void
 */
void terminal_draw_ascii_dunk_scene_two(void)
{
    terminal_writestring("   | o\\\n");
    terminal_writestring("|==|_  \\O/\n");
    terminal_writestring("|  'WW  |\n");
    terminal_writestring("         /\\\n");
    terminal_writestring("        /  \\\n");
}

/**
 * terminal_draw_ascii_dunk_scene_three - Draws a ASCII art dunk scene to the terminal
 *
 * Outputs a simple ASCII art representation of a basketball dunk scene,
 * including a figure performing a dunk with a basketball hoop.
 *
 * Returns: void
 */
void terminal_draw_ascii_dunk_scene_three(void)
{
    terminal_writestring("   |\n");
    terminal_writestring("|==O\n");
    terminal_writestring("|  |WW \\o/\n");
    terminal_writestring("        |\n");
    terminal_writestring("       / \\\n");
}
