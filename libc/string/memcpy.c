/**
 * @file memcpy.c
 * @brief Memory copy function.
 */

#include <string.h>

void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, const size_t size)
{
    unsigned char* dst = dstptr;
    const unsigned char* src = srcptr;
    for (size_t i = 0; i < size; i++)
    {
        dst[i] = src[i];
    }
    return dstptr;
}
