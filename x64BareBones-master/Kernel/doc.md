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