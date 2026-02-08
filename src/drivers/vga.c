//
// Created by soerenlemke on 01.02.26.
//

#include "vga.h"
#include "string.h"

/* ========================================================================
 * Private Variables
 * ======================================================================== */

uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;
terminal_cursor_position cursor_position = {.row = 0, .column = 0};

/* ========================================================================
 * Private Function Declarations
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
static uint8_t vga_entry_color(const enum vga_color fg, const enum vga_color bg)
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
static uint16_t vga_entry(const unsigned char uc, const uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

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
    if (++cursor_position.row == VGA_HEIGHT)
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
        cursor_position.row = VGA_HEIGHT - 1;
    }

    cursor_position.column = 0;
}

/**
 * @brief Writes a character entry to the VGA terminal buffer at the specified coordinates.
 *
 * Places a character at position (x, y) in the VGA terminal using the current terminal color.
 * If the coordinates exceed the terminal dimensions, they are clamped to the maximum valid values.
 *
 * @param c The character to write to the terminal buffer.
 * @param x The x-coordinate (column) where the character should be written.
 * @param y The y-coordinate (row) where the character should be written.
 *
 * @note Coordinates are automatically clamped to valid ranges:
 *       x is clamped to [0, VGA_WIDTH - 1]
 *       y is clamped to [0, VGA_HEIGHT - 1]
 */
static void terminal_putentryat(const char c, const size_t x, const size_t y)
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
    terminal_buffer[index] = vga_entry(c, terminal_color);
}

/* ========================================================================
 * Public Function Declarations
 * ======================================================================== */

/**
 * @brief Initializes the VGA terminal.
 *
 * Sets up the terminal by resetting the cursor position to (0, 0),
 * setting the default color to light gray text on a black background,
 * and clearing the entire terminal buffer by filling it with blank
 * characters in the default color.
 *
 * @return void
 */
void terminal_initialize(void)
{
    cursor_position.row = 0;
    cursor_position.column = 0;
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
        terminal_putchar(data[i], terminal_color);
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
 * @brief Sets the terminal text color by combining foreground and background colors.
 *
 * This function updates the global `terminal_color` variable by combining the
 * specified foreground and background colors into a single VGA color byte.
 *
 * @param foreground The foreground color (lower 4 bits of the VGA color byte).
 *                   This should be a value from the `vga_color` enum.
 * @param background The background color (upper 4 bits of the VGA color byte).
 *                   This should be a value from the `vga_color` enum.
 *
 * @return void
 */
void terminal_setcolor(const enum vga_color foreground, const enum vga_color background)
{
    terminal_color = vga_entry_color(foreground, background);
}

/**
 * @brief Clears the entire VGA terminal screen.
 *
 * This function resets the terminal display by filling the entire VGA buffer
 * with blank characters (' ') using the current terminal color. It also resets
 * the cursor position to the top-left corner of the screen (row 0, column 0).
 *
 * @return void
 */
void terminal_clear(void)
{
    for (size_t y = 0; y < VGA_HEIGHT; y++)
    {
        for (size_t x = 0; x < VGA_WIDTH; x++)
        {
            const size_t index = y * VGA_WIDTH + x;
            terminal_buffer[index] = vga_entry(' ', terminal_color);
        }
    }

    cursor_position.row = 0;
    cursor_position.column = 0;
}

/**
 * @brief Moves the cursor to the beginning of the next line.
 *
 * Scrolls the terminal content up if necessary when reaching the bottom.
 */
void terminal_newline(void)
{
    scroll_terminal();
}

/**
 * @brief Outputs a single character to the terminal display.
 *
 * Handles special characters (newline and carriage return) and writes
 * regular characters to the current cursor position. Automatically scrolls
 * the terminal when the end of a line is reached.
 *
 * @param c The character to output to the terminal.
 * @param color The color attribute to use for the character (foreground and background).
 *
 * @details
 * - '\n' (newline): Scrolls the terminal down.
 * - '\r' (carriage return): Resets the column to the start of the line.
 * - Other characters: Writes the character at the current cursor position
 *   using the current terminal color. If the column reaches VGA_WIDTH,
 *   the terminal is scrolled and the cursor wraps to the next line.
 */
void terminal_putchar(const char c, const uint8_t color)
{
    if (color)
    {
        terminal_color = color;
    }

    if (c == '\n')
    {
        scroll_terminal();
        return;
    }
    if (c == '\r')
    {
        cursor_position.column = 0;
        return;
    }

    terminal_putentryat(c, cursor_position.column, cursor_position.row);
    if (++cursor_position.column == VGA_WIDTH)
    {
        scroll_terminal();
    }
}

/**
 * @brief Sets the cursor position in the terminal.
 *
 * This function updates the cursor position in the terminal to the specified
 * row and column values. If the provided values exceed the terminal dimensions,
 * they are clamped to the maximum valid values.
 *
 * @param row The desired row position for the cursor (0-based index).
 * @param column The desired column position for the cursor (0-based index).
 *
 * @note The row value is clamped to the range [0, VGA_HEIGHT - 1].
 *       The column value is clamped to the range [0, VGA_WIDTH - 1].
 */
void terminal_set_cursor_position(const size_t row, const size_t column)
{
    size_t clamped_column = row;
    size_t clamped_row = column;

    if (clamped_column >= VGA_WIDTH)
    {
        clamped_column = VGA_WIDTH - 1;
    }

    if (clamped_row >= VGA_HEIGHT)
    {
        clamped_row = VGA_HEIGHT - 1;
    }

    cursor_position.row = clamped_row;
    cursor_position.column = clamped_column;
}