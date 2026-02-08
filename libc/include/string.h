/**
 * @file string.h
 * @brief String handling functions.
 *
 * This header provides string manipulation functions for the freestanding
 * C library. These functions work without an operating system.
 */

#ifndef _STRING_H
#define _STRING_H

#include <stddef.h>

/**
 * @brief Calculates the length of a string.
 *
 * @param str Pointer to a null-terminated string.
 * @return The number of characters before the null terminator.
 */
size_t strlen(const char* str);

/**
 * @brief Compares two memory blocks.
 *
 * @param aptr Pointer to first memory block.
 * @param bptr Pointer to second memory block.
 * @param size Number of bytes to compare.
 * @return 0 if equal, negative if aptr < bptr, positive if aptr > bptr.
 */
int memcmp(const void* aptr, const void* bptr, size_t size);

/**
 * @brief Copies memory from source to destination.
 *
 * @param dstptr Destination pointer.
 * @param srcptr Source pointer.
 * @param size Number of bytes to copy.
 * @return The destination pointer.
 */
void* memcpy(void* __restrict dstptr, const void* __restrict srcptr, size_t size);

/**
 * @brief Copies memory (handles overlapping regions).
 *
 * @param dstptr Destination pointer.
 * @param srcptr Source pointer.
 * @param size Number of bytes to copy.
 * @return The destination pointer.
 */
void* memmove(void* dstptr, const void* srcptr, size_t size);

/**
 * @brief Fills memory with a constant byte.
 *
 * @param bufptr Pointer to memory block.
 * @param value Value to set (converted to unsigned char).
 * @param size Number of bytes to set.
 * @return The buffer pointer.
 */
void* memset(void* bufptr, int value, size_t size);

#endif /* _STRING_H */
