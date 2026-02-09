# QEMU `-kernel` vs. ISO with GRUB

### Problem

- Running with `qemu-system-i386 -kernel` causes error: `multiboot knows VBE. we don't`

### Cause

- `VIDMODE` flag in Multiboot header (`boot.s`) requests VBE framebuffer
- QEMU's `-kernel` mode loads kernel directly without bootloader
- No VBE support without GRUB

### Solution

| Mode      | Bootloader         | Video Mode           | Makefile Target |
|-----------|--------------------|----------------------|-----------------|
| `-kernel` | None (QEMU direct) | VGA Text (0xB8000)   | `run-text`      |
| `-cdrom`  | GRUB               | Framebuffer possible | `run`           |

- For quick tests without Docker: remove `VIDMODE` from FLAGS
- ISO with GRUB required for framebuffer features

### Key Takeaways

- `-kernel` = simplified boot process, no VBE
- `-cdrom` with ISO = real bootloader, full Multiboot support
- Multiboot header flags only work when bootloader interprets them

### References

- [QEMU Invocation](https://www.qemu.org/docs/master/system/invocation.html)
- [Multiboot Specification](https://www.gnu.org/software/grub/manual/multiboot/multiboot.html)
