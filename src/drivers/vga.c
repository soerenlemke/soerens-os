//
// Created by soerenlemke on 01.02.26.
//

#include "vga.h"
#include "string.h"

// Variable definitions
size_t terminal_row;
size_t terminal_column;
uint8_t terminal_color;
uint16_t* terminal_buffer = (uint16_t*)VGA_MEMORY;

void scroll_terminal(void);
void terminal_putentryat(char c, uint8_t color, size_t x, size_t y);

uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg)
{
    return fg | bg << 4;
}

uint16_t vga_entry(const unsigned char uc, const uint8_t color)
{
    return (uint16_t)uc | (uint16_t)color << 8;
}

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

void terminal_setcolor(const uint8_t color)
{
    terminal_color = color;
}

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

void terminal_write(const char* data, const size_t size)
{
    for (size_t i = 0; i < size; i++)
        terminal_putchar(data[i]);
}

void terminal_writestring(const char* data)
{
    terminal_write(data, strlen(data));
}

void terminal_putentryat(const char c, const uint8_t color, const size_t x, const size_t y)
{
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

// In case the terminal is filled up, move all rows up one row and discard the uppermost, and
// leave a blank row at the bottom ready to be filled up with characters.
void scroll_terminal(void)
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