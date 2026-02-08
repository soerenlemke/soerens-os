# Dockerfile for building SoerensOS ISO
# Usage: docker build -t soerens-os-builder . && docker run --rm -v $(pwd):/src soerens-os-builder

# Force amd64 platform for grub-pc-bin compatibility (required for Apple Silicon)
ARG TARGETPLATFORM=linux/amd64
FROM --platform=$TARGETPLATFORM debian:bookworm-slim

RUN apt-get update && apt-get install -y \
    build-essential \
    bison \
    flex \
    libgmp3-dev \
    libmpc-dev \
    libmpfr-dev \
    texinfo \
    grub-pc-bin \
    grub-common \
    xorriso \
    mtools \
    curl \
    && rm -rf /var/lib/apt/lists/*

# Build cross-compiler
ENV PREFIX=/opt/cross
ENV TARGET=i686-elf
ENV PATH="$PREFIX/bin:$PATH"

WORKDIR /tmp

# Binutils
RUN curl -O https://ftp.gnu.org/gnu/binutils/binutils-2.42.tar.xz \
    && tar -xf binutils-2.42.tar.xz \
    && mkdir binutils-build && cd binutils-build \
    && ../binutils-2.42/configure --target=$TARGET --prefix=$PREFIX --with-sysroot --disable-nls --disable-werror \
    && make -j$(nproc) && make install \
    && cd .. && rm -rf binutils*

# GCC
RUN curl -O https://ftp.gnu.org/gnu/gcc/gcc-14.1.0/gcc-14.1.0.tar.xz \
    && tar -xf gcc-14.1.0.tar.xz \
    && mkdir gcc-build && cd gcc-build \
    && ../gcc-14.1.0/configure --target=$TARGET --prefix=$PREFIX --disable-nls --enable-languages=c --without-headers \
    && make -j$(nproc) all-gcc all-target-libgcc \
    && make install-gcc install-target-libgcc \
    && cd .. && rm -rf gcc*

WORKDIR /src

# Default command: build ISO
CMD ["make", "clean", "iso"]

