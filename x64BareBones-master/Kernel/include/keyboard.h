#ifndef KEYBOARD_H
#define KEYBOARD_H

#include <video.h>
#include <naiveConsole.h>
#include <lib.h>
#include <snapshot.h>

// Tamaño de los arrays de scancodes
#define SCANCODE_MAP_SIZE 256

#define BUFFER_DIM 256

// No hay carácter imprimible
#define KC_NONE   0

// Teclas no imprimibles
#define KC_ENTER  '\n'
#define KC_BACKSP '\b'
#define KC_TAB    '\t'
#define KC_ESC    0x1B
#define KC_SPACE  ' '

#define DELTA ('a' - 'A')

// Estados especiales
#define UNPRESSED_BIT     0x80  // Bit que indica break code (tecla soltada)
#define CAPS_LOCK         0x3A
#define LEFT_SHIFT        0x2A
#define RIGHT_SHIFT       0x36
#define SNAPSHOT_KEY      0x29

void keyboardHandler();

#endif