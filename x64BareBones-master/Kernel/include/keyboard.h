#ifndef KEYBOARD_H
#define KEYBOARD_H


// Puertos de E/S del teclado
#define KEYBOARD_DATA_PORT    0x60  // Puerto de datos del teclado
#define KEYBOARD_STATUS_PORT  0x64  // Puerto de estado del teclado

// Tamaño de los arrays de scancodes
#define SCANCODE_MAP_SIZE 256

// No hay carácter imprimible
#define KC_NONE   0

// Teclas no imprimibles
#define KC_ENTER  '\n'
#define KC_BACKSP '\b'
#define KC_TAB    '\t'
#define KC_ESC    0x1B
#define KC_SPACE  ' '

// Estados especiales
#define UNPRESSED_BIT     0x80  // Bit que indica break code (tecla soltada)
#define CAPS_LOCK         0x3A
#define LEFT_SHIFT        0x2A
#define RIGHT_SHIFT       0x36

void keyboard_handler();

#endif