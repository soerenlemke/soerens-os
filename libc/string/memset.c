/**
 * @file memset.c
 * @brief Memory set function.
 */

#include <string.h>

void* memset(void* bufptr, const int value, const size_t size)
{
    unsigned char* buf = bufptr;
    for (size_t i = 0; i < size; i++)
    {
        buf[i] = (unsigned char)value;
    }
    return bufptr;
}
