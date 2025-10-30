# Documentación Kernel - Estructura

## Descripción general
El kernel es el corazón del sistema operativo.

Se encarga de comunicar el software con el hardware, administrar recursos (CPU, memoria, discos, etc.) y mantener el sistema funcionando de forma segura y ordenada.

Es lo que el bootloader carga en memoria para que el sistema operativo pueda comenzar a funcionar.
---

## Estructura 
Kernel/
|____ loader.asm    # Inicializa el Kernel (initializeKernelBinary), setea el SP y llama al main()
|____ kernel.c      # Implementa main() (entre otros)
|____ idtLoader.c   # Implementa load_idt() -> (Setea las entradas de la IDT)
|____ asm/
    |____ interrupts.asm    # Define los handlers para cada interrupcion (software y hardware)
    |____ ...
|____ irqDispatcher.c   # Ejecuta las ISR
|____ drivers/          # Implementa drivers de teclado y video
    |____ keyboard.c
    |____ video.c
|____ include/
    |____ ...
...cont
---

## Flujo
main
    -> *module

loader 
    -> initializeKernelBinary
        -> loadModules
            -> (for) loadModule
        -> clearBSS
        -> (ret) getStackBase
    -> main
        -> load_idt 
            -> setup_IDT_entry 
            -> picMasterMask
            -> picSlaveMask

*hardware interrupt* 
    -> irqXXHandler
        -> irqHandlerMaster X
            -> irqDispatcher X
                -> int_X        
                    -> X_handler
                        -> ...
---

## Module Loader
loadModules carga módulos en memoria.
La función recibe dos parámetros:
    - payloadStart: Puntero al inicio donde están los módulos en memoria
    - targetModuleAddress: Array de direcciones donde se copiarán los módulos

1. Lee cuántos módulos hay (moduleCount) usando readUint32
2. Para cada módulo:
   - Lee su tamaño
   - Copia el módulo desde su ubicación original a la dirección destino usando memcpy()
   - Avanza el puntero al siguiente módulo
---

## System Calls
[System Calls are used to call a kernel service from user land](https://wiki.osdev.org/System_Calls)

- idtLoader.c (entry _int80Handler)
- interrupts.asm (implementa _int80Handler -> llama a syscallDispatcher)
- syscalls.c (implementa syscallDispatcher -> llama a la función correspondiente segun el syscall_id)
---

### Shell
https://wiki.osdev.org/Creating_A_Shell
"As a shell is a form of user interface, it will typically read input from the user (which may, as aforementioned, occur through its "standard input" stream) and display output to the user. It's important, however, to get the distinction between the physical hardware used to interact with the user and the shell's interface with the user."

Typically, thus, a simple shell will do the following for each binary executed:

- Read a command from the user, which becomes the name of the binary to execute plus any command-line arguments (if applicable)
- Tell the kernel to execute the binary
- Read input from the user and send it to the binary
- Read output from the binary and send it to the user
- Repeat steps 3 and 4 until the binary terminates

In order to do this, the shell will require the following:

- A kernel with an API call for executing a binary
- A way to get input from the user (typically, this requires a keyboard driver for the kernel and an API call for reading from the keyboard - may take the form of "standard input")
- A way to send output to the user (typically, this requires a video driver for the kernel and an API call for printing text to the screen - may take the form of "standard output")
---