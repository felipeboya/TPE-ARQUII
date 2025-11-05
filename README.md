### Custom x86-64 Kernel

The base code for this project can be found at:  
https://github.com/alejoaquili/x64BareBones

The example project includes the following components:
- Bootloader – low-level startup code that initializes the system  
- Image – prebuilt disk images for emulators  
- Kernel – main operating system code  
- Toolchain – packaging and build tools  
- Userland – user applications that act as OS clients  

Overview

This project implements a **custom 64-bit operating system kernel**, bootable using **Pure64**, for **Intel x86-64 Long Mode**.  
It manages hardware resources and exposes a simple **API for user-space programs** to interact with the system safely.

The kernel includes:

- Interrupt and exception handling  
- Basic system calls (`read`, `write`, etc.)  
- Keyboard and video drivers  
- Timer and clock management  
- A simple **shell** interface for user interaction  


Boot Process

1. **Pure64** loads the kernel into memory and switches to Long Mode.  
2. The **IDT** (Interrupt Descriptor Table) and interrupt handlers are initialized.  
3. The **drivers** (keyboard, timer, video) are loaded.  
4. The **system call interface (INT 0x80)** is configured.  
5. Control is passed to the **userland shell**.


Environment Setup

On Ubuntu / Linux, to install QEMU and its utilities, run:
```bash
sudo apt install qemu-system-x86 qemu-utils
```
If you’re using macOS with Homebrew, install QEMU with:
```bash
brew install qemu
```

Building and Running

To build and run the project, simply execute:
```bash
./compile.sh
./run.sh
