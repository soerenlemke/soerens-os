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

    while (1)
    {
        terminal_draw_ascii_dunk_scene_one();
        delay(DELAY_COUNT);
        terminal_initialize();

        terminal_draw_ascii_dunk_scene_two();
        delay(DELAY_COUNT);
        terminal_initialize();

        terminal_draw_ascii_dunk_scene_three();
        delay(DELAY_COUNT);
        terminal_initialize();
    }
}
