/**
 * @file memmove.c
 * @brief Memory move function (handles overlapping regions).
 */

#include <string.h>

void* memmove(void* dstptr, const void* srcptr, const size_t size)
{
    unsigned char* dst = dstptr;
    const unsigned char* src = srcptr;
    if (dst < src)
    {
        for (size_t i = 0; i < size; i++)
        {
            dst[i] = src[i];
        }
    }
    else
    {
        for (size_t i = size; i != 0; i--)
        {
            dst[i - 1] = src[i - 1];
        }
    }
    return dstptr;
}
