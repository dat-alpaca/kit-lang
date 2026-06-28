# KIT Lang
> This assembly language is based on the Kyoto Institute of Technology Programming II lectures and is only meant to be used for educational purposes. There are many issues in this codebase that will not be addressed, **such as optimization**.

KIT is an x86-inspired assembly language based on my university's programming course on assembly languages. It provides the following minimal instruction set:

- COPY r64, imm32
- COPY r64, mem
- COPY mem, r64
- ADD r64, imm32
- ADD r64, mem
- MUL r64
- CMP r64, mem
- JB label
- JNB label
- JMP label
- IN r64
- OUT r64

Currently, four 64-bit registers (r64) are available: `AX`, `BX`, `CX`, and `DX`.

> [!WARNING]
> This project is currently only available on Linux. I may add support for generating Windows executables in the future.

# Getting Started
Like my previous projects, this one doesn't have prebuilt binaries. So, you will need to build it yourself (sorry).

The latest version of this project has only been tested on [Fedora Linux 43 (KDE Plasma Edition)](https://www.fedoraproject.org/).

## Prerequisites
If you actually want to build this thing, you are going to need:

* [Meson](https://mesonbuild.com/index.html)

## Installation

1. **Clone the repository**
    ```bash
    git clone https://github.com/dat-alpaca/kit-lang
    ```
    <br>

2. **Set up the project**
    This project doesn't have any build options. All you need to do is set it up:
    
    ```bash
    ./scripts/setup
    ./scripts/configure
    ```

3. **Build and Run**
    The following command will compile the project, create a `deploy` folder, and copy the assembler into it:
    ```bash
    ./scripts/build
    ```

    That's all you need to do. To test the assembler, copy the included `test.kit` file:

    ```bash
    cp test.kit deploy/test.kit
    cd deploy
    ./kit-assembler test.kit
    chmod +x ./main
    ./main
    ```

# Sources
These are the awesome resources that I have used to build this assembler:

- [x86 and amd64 instruction reference](https://www.felixcloutier.com/x86/)
- [ELF Format Cheatsheet](https://gist.github.com/x0nu11byt3/bcb35c3de461e5fb66173071a2379779)
- [ELF from scratch](https://www.conradk.com/elf-from-scratch/)

The overall design of this project is based on a book I read called [Crafting Interpreters](https://craftinginterpreters.com/contents.html). I'm not sure whether production-grade assemblers work this way, but I hope my code isn't too far off.

# License
This project is licensed under the [MIT License](LICENSE).