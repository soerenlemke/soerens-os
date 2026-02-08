# Mac Setup for soerens-os

## Quick Start (if cross-compiler is already installed)

```bash
# 1. Set PATH (once per terminal, or add to ~/.zshrc)
export PATH="$HOME/opt/cross/bin:$PATH"

# 2. Build the project
make

# 3. Run OS in QEMU
make run-iso
```

---

## First-Time Setup

### 1. Install Homebrew packages

```bash
brew install gmp mpfr libmpc xorriso qemu nasm objconv
```

### 2. Build the i686-elf Cross-Compiler

Reference: https://wiki.osdev.org/GCC_Cross-Compiler

```bash
export PREFIX="$HOME/opt/cross"
export TARGET=i686-elf
export PATH="$PREFIX/bin:$PATH"

mkdir -p ~/src && cd ~/src
```

#### Binutils

```bash
curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.43.1.tar.xz
tar -xf binutils-2.43.1.tar.xz
mkdir -p build/binutils && cd build/binutils
../../binutils-2.43.1/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror
make -j$(sysctl -n hw.ncpu)
make install
```

#### GCC

```bash
cd ~/src
curl -O https://ftp.gnu.org/gnu/gcc/gcc-14.2.0/gcc-14.2.0.tar.xz
tar -xf gcc-14.2.0.tar.xz
mkdir -p build/gcc && cd build/gcc
../../gcc-14.2.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c,c++ --without-headers
make -j$(sysctl -n hw.ncpu) all-gcc
make -j$(sysctl -n hw.ncpu) all-target-libgcc
make install-gcc
make install-target-libgcc
```

### 3. Set PATH permanently

Add to `~/.zshrc`:

```bash
export PATH="$HOME/opt/cross/bin:$PATH"
```

Then restart the terminal or run `source ~/.zshrc`.

### 4. Build GRUB for ISO creation

GRUB is needed to create bootable ISO images:

```bash
make build-grub
```

This will download and compile GRUB 2.06 to `~/opt/grub`. This is a one-time setup.

### 5. Verify installation

```bash
i686-elf-gcc --version
~/opt/grub/bin/grub-mkrescue --version
```

---

## Make Commands

| Command           | Description                 |
|-------------------|-----------------------------|
| `make`            | Compile the kernel          |
| `make iso`        | Create ISO image            |
| `make run-iso`    | Run ISO in QEMU             |
| `make run-kernel` | Run kernel directly in QEMU |
| `make clean`      | Delete build files          |

