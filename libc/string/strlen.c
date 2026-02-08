/**
 * @file strlen.c
 * @brief String length calculation.
 */

#include <string.h>

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
    {
        len++;
    }
    return len;
}
