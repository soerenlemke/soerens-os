#include "vga.h"

#include <stddef.h>
#include <stdint.h>

/* Check if the compiler thinks you are targeting the wrong operating system. */
#if defined(__linux__)
#error "You are not using a cross-compiler, you will most certainly run into trouble"
#endif

/* This tutorial will only work for the 32-bit ix86 targets. */
#if !defined(__i386__)
#error "This tutorial needs to be compiled with a ix86-elf compiler"
#endif

// used to see terminal scrolling
void delay(const uint32_t count)
{
    for (volatile uint32_t i = 0; i < count; i++)
    {
        // volatile prevents compiler optimization
    }
}

void kernel_main(void)
{
    /* Initialize terminal interface */
    terminal_initialize();

    uint8_t color_index = 0;
    const uint8_t num_colors = 16;

    while (1)
    {
        uint8_t bg_color = color_index % num_colors;
        uint8_t fg_color = (color_index + 8) % num_colors;

        terminal_setcolor(bg_color, fg_color);
        terminal_writestring("   |    o\\\n");
        terminal_writestring("|==|__    O\n");
        terminal_writestring("|  'WW    /\\\n");
        terminal_writestring("           /\\__\n");
        terminal_writestring("           \\\n");
        delay(DELAY_COUNT);
        terminal_initialize();

        terminal_setcolor(fg_color, bg_color);
        terminal_writestring("   | o\\\n");
        terminal_writestring("|==|_  \\O/\n");
        terminal_writestring("|  'WW  |\n");
        terminal_writestring("         /\\\n");
        terminal_writestring("        /  \\\n");
        delay(DELAY_COUNT);
        terminal_initialize();

        terminal_setcolor(bg_color, fg_color);
        terminal_writestring("   |\n");
        terminal_writestring("|==O\n");
        terminal_writestring("|  |WW \\o/\n");
        terminal_writestring("        |\n");
        terminal_writestring("       / \\\n");
        delay(DELAY_COUNT);
        terminal_initialize();

        color_index++;
    }
}

