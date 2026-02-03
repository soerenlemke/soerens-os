//
// Created by soerenlemke on 02.02.26.
//

#include "string.h"

size_t strlen(const char* str)
{
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}