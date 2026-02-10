/**
* @file multiboot.h
 * @brief Multiboot specification structures and constants.
 *
 * This header defines the Multiboot structures and constants
 * according to the Multiboot specification version 1.
 */


#ifndef _KERNEL_MULTIBOOT_H
#define _KERNEL_MULTIBOOT_H

#include "types.h"

/**
 * @brief Magic number passed by a Multiboot-compliant bootloader.
 *
 * This value is used by the kernel to verify that it was loaded by a
 * Multiboot-compliant bootloader. The bootloader places this value in the
 * `eax` register before transferring control to the kernel.
 */
#define MULTIBOOT_BOOTLOADER_MAGIC 0x2BADB002

/**
 * @brief Flag indicating the presence of framebuffer information.
 *
 * If this flag is set in the `flags` field of the `multiboot_info` structure,
 * it indicates that the bootloader has provided information about the
 * framebuffer (e.g., for graphical mode support).
 */
#define MULTIBOOT_INFO_FRAMEBUFFER 0x1000

/**
 * @brief Structure representing the Multiboot information provided by the bootloader.
 *
 * This structure is defined according to the Multiboot specification version 1.
 * It contains various fields that provide information about the system's state
 * and configuration at the time the bootloader transfers control to the kernel.
 *
 * The `__attribute__((packed))` directive ensures that the structure is packed
 * without padding, as required by the Multiboot specification.
 */
typedef struct multiboot_info
{
    uint32_t flags; /**< Bitfield indicating which fields are valid. */
    uint32_t mem_lower; /**< Amount of lower memory (in kilobytes). */
    uint32_t mem_upper; /**< Amount of upper memory (in kilobytes). */
    uint32_t boot_device; /**< Boot device identifier. */
    uint32_t cmdline; /**< Physical address of the kernel command line. */
    uint32_t mods_count; /**< Number of loaded modules. */
    uint32_t mods_addr; /**< Physical address of the first module structure. */
    uint32_t syms[4]; /**< Symbol table or section header table (ELF format). */
    uint32_t mmap_length; /**< Length of the memory map. */
    uint32_t mmap_addr; /**< Physical address of the memory map. */
    uint32_t drives_length; /**< Length of the drive structure information. */
    uint32_t drives_addr; /**< Physical address of the first drive structure. */
    uint32_t config_table; /**< Physical address of the ROM configuration table. */
    uint32_t boot_loader_name; /**< Physical address of the bootloader name. */
    uint32_t apm_table; /**< Physical address of the APM table. */
    uint32_t vbe_control_info; /**< Physical address of the VBE control information. */
    uint32_t vbe_mode_info; /**< Physical address of the VBE mode information. */
    uint16_t vbe_mode; /**< Current VBE mode. */
} __attribute__((packed)) multiboot_info_t;

#endif //_KERNEL_MULTIBOOT_H
