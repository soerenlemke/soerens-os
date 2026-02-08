//
// Created by soerenlemke on 01.02.26.
//

#ifndef VGA_H
#define VGA_H

#include <stddef.h>
#include <stdint.h>

#define VGA_WIDTH 80
#define VGA_HEIGHT 25
#define VGA_MEMORY 0xB8000
#define DELAY_COUNT 50000000 // Adjust value depending on CPU speed

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
 * @brief Structure representing the cursor position in the terminal.
 *
 * This structure stores the current position of the cursor in the terminal
 * as row and column indices.
 */
typedef struct
{
    size_t row;    /**< The row position of the cursor in the terminal. */
    size_t column; /**< The column position of the cursor in the terminal. */
} terminal_cursor_position;

extern uint8_t terminal_color;
extern uint16_t* terminal_buffer;
extern terminal_cursor_position cursor_position;

void terminal_initialize(void);
void terminal_setcolor(enum vga_color foreground, enum vga_color background);
void terminal_write(const char* data, size_t size);
void terminal_writestring(const char* data);
void terminal_clear(void);
void terminal_newline(void);
void terminal_putchar(char c, uint8_t color);
void terminal_set_cursor_position(size_t row, size_t column);

#endif // VGA_H
