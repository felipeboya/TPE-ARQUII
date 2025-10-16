#include <keyboard.h>

#include <stdint.h>

// No hay carácter imprimible
#define KC_NONE   0

// Teclas no imprimibles
#define KC_ENTER  '\n'
#define KC_BACKSP '\b'
#define KC_TAB    '\t'
#define KC_ESC    0x1B
#define KC_SPACE  ' '


// Mapeo del Scan Code Set 1 a caracteres 
static const char scancode1_to_char[256] = {
    [0x01] = KC_ESC,
    [0x02] = '1',
    [0x03] = '2',
    [0x04] = '3',
    [0x05] = '4',
    [0x06] = '5',
    [0x07] = '6',
    [0x08] = '7',
    [0x09] = '8',
    [0x0A] = '9',
    [0x0B] = '0',
    [0x0C] = '-',
    [0x0D] = '=',
    [0x0E] = KC_BACKSP,
    [0x0F] = KC_TAB,
    [0x10] = 'q',
    [0x11] = 'w',
    [0x12] = 'e',
    [0x13] = 'r',
    [0x14] = 't',
    [0x15] = 'y',
    [0x16] = 'u',
    [0x17] = 'i',
    [0x18] = 'o',
    [0x19] = 'p',
    [0x1A] = '[',
    [0x1B] = ']',
    [0x1C] = KC_ENTER,
    [0x1D] = KC_NONE,    // Ctrl
    [0x1E] = 'a',
    [0x1F] = 's',
    [0x20] = 'd',
    [0x21] = 'f',
    [0x22] = 'g',
    [0x23] = 'h',
    [0x24] = 'j',
    [0x25] = 'k',
    [0x26] = 'l',
    [0x27] = ';',
    [0x28] = '\'',
    [0x29] = '`',
    [0x2A] = KC_NONE,    // Left Shift
    [0x2B] = '\\',
    [0x2C] = 'z',
    [0x2D] = 'x',
    [0x2E] = 'c',
    [0x2F] = 'v',
    [0x30] = 'b',
    [0x31] = 'n',
    [0x32] = 'm',
    [0x33] = ',',
    [0x34] = '.',
    [0x35] = '/',
    [0x36] = KC_NONE,   
    [0x37] = '*',       
    [0x38] = KC_NONE,    
    [0x39] = KC_SPACE,
    [0x3A] = KC_NONE,    
    [0x3B] = KC_NONE, [0x3C] = KC_NONE, [0x3D] = KC_NONE, [0x3E] = KC_NONE,
    [0x3F] = KC_NONE, [0x40] = KC_NONE, [0x41] = KC_NONE, [0x42] = KC_NONE,
    [0x43] = KC_NONE, [0x44] = KC_NONE, [0x45] = KC_NONE, [0x46] = KC_NONE,
    [0x47] = '7', [0x48] = '8', [0x49] = '9', [0x4A] = '-', [0x4B] = '4',
    [0x4C] = '5', [0x4D] = '6', [0x4E] = '+', [0x4F] = '1', [0x50] = '2',
    [0x51] = '3', [0x52] = '0', [0x53] = '.', 
    [0x57] = KC_NONE,    // F11
    [0x58] = KC_NONE,    // F12
};

char scancode1_to_key(uint8_t scancode) {
    return scancode1_to_char[scancode];
}

int getKey();

void keyboard_handler() {
    unsigned char scancode = getKey();

    scancode=getKey();
	int teclahex=translate(scancode);
	if (teclahex >= 'a' && teclahex <= 'z' || teclahex == ' ')
		ncPrintChar(teclahex);
}
