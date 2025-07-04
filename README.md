# CatOS

CatOS is a minimalist operating system project, developed mainly in Makefile, C, and Assembly. The primary goal of this project is educational—to experiment and learn the basics of building an operating system from scratch.

## Table of Contents

- [About](#about)
- [Features](#features)
- [Requirements](#requirements)
- [Building](#building)
- [Running](#running)
- [Directory Structure](#directory-structure)
- [License](#license)
- [Author](#author)

## About

CatOS is a hobby operating system project for educational purposes. The repository contains the source code for the bootloader, kernel, configuration files, and build scripts.

## Features

- Simple bootloader
- Basic kernel written in C and Assembly
- (In progress) Memory management
- Basic keyboard and screen I/O
- Can be run in the QEMU emulator

## Requirements

- `gcc` and `nasm` compilers
- `make`
- (Optional) QEMU emulator

## Building

To build the project, run:

```sh
make
```

All build artifacts will appear in the `build/` directory.

## Running

To run the system in QEMU:

```sh
make run
```

Or run the generated ISO image in your preferred emulator.

## Directory Structure

- `src/` – Source code for the kernel and bootloader
- `build/` – Build output files
- `Makefile` – Project build script

## License

This project is licensed under the MIT License.

## Author

Developed by [Kitler174](https://github.com/Kitler174).
