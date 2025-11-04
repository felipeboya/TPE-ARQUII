#include <keyboard.h>

typedef struct {
    uint8_t keysBuffer[BUFFER_DIM];   // (buffer circular)
    uint64_t writeIndex;              // donde se escribe el proximo
    uint64_t readIndex;               // donde se lee el proximo
    uint64_t size;                    // cantidad de keys en el buffer
} keyboardBuffer;

static keyboardBuffer kb = {0};

// Variables de estado del teclado
static uint8_t caps_lock = 0;
static uint8_t shift_pressed = 0;

static uint8_t getScancode();
static const uint8_t scancodesMap[SCANCODE_MAP_SIZE];
static const uint8_t shift_map[SCANCODE_MAP_SIZE];
static uint8_t scancodeToChar(uint8_t scancode);
static uint8_t handleKey(uint8_t scancode);

void keyboardHandler() {
    uint8_t scancode = getScancode();

    uint8_t c = handleKey(scancode);
    if( c == KC_NONE ){   
        return;
    }

    kb.keysBuffer[kb.writeIndex] = c;
    kb.writeIndex = (kb.writeIndex + 1) % BUFFER_DIM;
    if (kb.size < BUFFER_DIM) {
        kb.size++;
    } else {
        // avanzar readIndex para mantener tamaño constante y sobrescribir el más antiguo
        kb.readIndex = (kb.readIndex + 1) % BUFFER_DIM;
    }

    return;
}

uint8_t getChar(){
    if ( kb.size == 0 ){
        return 0; 
    }
    uint8_t c = kb.keysBuffer[kb.readIndex];
    kb.readIndex = (kb.readIndex + 1) % BUFFER_DIM;
    kb.size--; 
    return c; 
}

// Funciones Estáticas 

// Lee un scancode del puerto de teclado
static uint8_t getScancode() {
    return getKey();
}

// Mapeo de scancode a caracteres minúsculas
static const uint8_t scancodesMap[SCANCODE_MAP_SIZE] = {
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
    [0x36] = KC_NONE,    // Right Shift
    [0x37] = '*',       
    [0x38] = KC_NONE,    // Alt
    [0x39] = KC_SPACE,
    [0x3A] = KC_NONE,    // Caps Lock
    [0x3B] = KC_NONE,    // F1
    [0x3C] = KC_NONE, [0x3D] = KC_NONE, [0x3E] = KC_NONE,
    [0x3F] = KC_NONE, [0x40] = KC_NONE, [0x41] = KC_NONE, [0x42] = KC_NONE,
    [0x43] = KC_NONE, [0x44] = KC_NONE, [0x45] = KC_NONE, [0x46] = KC_NONE,
    [0x47] = '7', [0x48] = '8', [0x49] = '9', [0x4A] = '-', [0x4B] = '4',
    [0x4C] = '5', [0x4D] = '6', [0x4E] = '+', [0x4F] = '1', [0x50] = '2',   
    [0x51] = '3', [0x52] = '0', [0x53] = '.', 
    [0x57] = KC_NONE,    // F11
    [0x58] = KC_NONE,    // F12
};

// Mapeo de caracteres con shift presionado
static const uint8_t shift_map[SCANCODE_MAP_SIZE] = {
    ['1'] = '!', ['2'] = '@', ['3'] = '#', ['4'] = '$', ['5'] = '%',
    ['6'] = '^', ['7'] = '&', ['8'] = '*', ['9'] = '(', ['0'] = ')',
    ['-'] = '_', ['='] = '+', ['['] = '{', [']'] = '}', ['\\'] = '|',
    [';'] = ':', ['\''] = '"', [','] = '<', ['.'] = '>', ['/'] = '?',
    ['`'] = '~'
};

// Convierte un scancode a su carácter correspondiente
static uint8_t scancodeToChar(uint8_t scancode) {
    uint8_t c = scancodesMap[scancode];
    
    if (c >= 'a' && c <= 'z') {
        // Aplicar mayúsculas si corresponde
        if ((shift_pressed && !caps_lock) || (!shift_pressed && caps_lock)) {
            c = c - 'a' + 'A';
        }
    }
    // Si es otro carácter y shift está presionado
    else if (shift_pressed && shift_map[(unsigned char)c]) {
        c = shift_map[(unsigned char)c];
    }
    
    return c;
}

// Retorna el scancode como char y maneja las teclas especiales
uint8_t handleKey(uint8_t scancode){
    // Verifica si es una tecla liberada
    if (scancode >= UNPRESSED_BIT) {
        scancode -= UNPRESSED_BIT;
        
        if (scancode == LEFT_SHIFT || scancode == RIGHT_SHIFT) {
            shift_pressed = 0;
        }
        return KC_NONE;
    }
    
    if (scancode == SNAPSHOT_KEY){
        snapshot();
        return KC_NONE;
    }

    switch (scancode) {
        case CAPS_LOCK:
            caps_lock = !caps_lock;
            return KC_NONE;
        case LEFT_SHIFT:
        case RIGHT_SHIFT:
            shift_pressed = 1;
            return KC_NONE;
        default:
            break; 
    }
    
    uint8_t c = scancodeToChar(scancode);

    if(c == KC_ESC || c == KC_NONE){
        return KC_NONE;
    }

    return c;
}