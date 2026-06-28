# KIT Lang
> This assembly language is based on the Kyoto Institute of Technology Programming II lectures and is only meant to be used for educational purposes. There are many issues in this codebase which will not be addressed. 

The KIT language is a assembly language based on x86 and provides a miniscule instruction set. Many instructions have not been implemented, but a list can be found in the following section.

- [X] COPY AX, imm32
- [x] COPY AX, mem
- [ ] COPY mem, AX
- [x] ADD AX, imm32
- [x] ADD AX, mem
- [x] MUL reg
- [ ] MUL AX, imm32
- [ ] MUL AX, mem
- [x] CMP AX, mem
- [x] JB label
- [x] JNB label
- [x] JMP label
- [x] IN AX
- [x] OUT AX
- [ ] CF flag (uses `stc` and `clc`)

> [!WARNING]
> This project is currently only available on Linux. I may implement a EXE header in the future.

# Getting Started
As is the case with all my previous projects, no prebuilt binaries have been released at the time of writing. If you want to use it, you'll need to build it yourself (sorry).

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

2. **Setup the project**
    This project has no build flags. All you need to do is set it up:
    
    ```bash
    ./scripts/setup
    ./scripts/configure
    ```

3. **Build and Run**
    The following command will compile the project, create a `deploy` folder, and copy the assembler into it.
    ```bash
    ./scripts/build
    ```

    That's all you need to do. If you want to test the application, I have provided a `test.kit` file. You can use the code below to test it out:

    ```bash
    cp test.kit deploy/test.kit
    cd deploy
    ./kit-assembler test.kit
    chmod +x ./main
    ./main
    ```

# License
This project is licensed under the [MIT License](LICENSE).