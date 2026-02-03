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

    // test for terminal scrolling
    for (int i = 0; i < 30; i++)
    {
        terminal_writestring("Line ");
        terminal_putchar('0' + (i / 10));
        terminal_putchar('0' + (i % 10));
        terminal_putchar('\n');
        delay(DELAY_COUNT);
    }
}
