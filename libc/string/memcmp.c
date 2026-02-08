/**
 * @file memcmp.c
 * @brief Memory comparison function.
 */

#include <string.h>

int memcmp(const void* aptr, const void* bptr, const size_t size)
{
    const unsigned char* a = aptr;
    const unsigned char* b = bptr;
    for (size_t i = 0; i < size; i++)
    {
        if (a[i] < b[i])
        {
            return -1;
        }
        if (a[i] > b[i])
        {
            return 1;
        }
    }
    return 0;
}
