#include <exceptions.h>

static uint8_t * numToHex(uint64_t num); 
static void printRegs(uint8_t * title);
static void printTitle(uint8_t * title);
static uint64_t strlen(const char * str);

uint64_t registersArrayExceptions[CANT_REGS];


static uint8_t * numToHex(uint64_t num) {
    static char buffer[17];
    uint8_t * ptr = &buffer[16];
    *ptr = '\0';
    for(int i = 0; i < 16; i++) {
        *--ptr = "0123456789ABCDEF"[num & 0xF];
        num >>= 4;
    }
    return ptr;
}

static uint64_t strlen(const char *s) {
    uint64_t i = 0;
    while (*s != 0) {
        i++;
        s++;
    }
    return i;
}

static void printTitle(uint8_t * title) {
    uint64_t titleLen = strlen(title);
    textWrite(STDERR, "\n", 1);
    textWrite(STDERR, " ", 1);
    textWrite(STDERR, title, titleLen);
    textWrite(STDERR, "\n", 1);
    textWrite(STDERR, " ", 1);
    for(uint64_t i = 0; i < titleLen; i++) {
        textWrite(STDERR, "=", 1);
    }
    textWrite(STDERR, "\n\n", 2);
}

static void printRegs(uint8_t * title) {
    uint8_t * regs[] = {
        "RAX", "RBX", "RCX", "RDX", "RBP", "RDI", "RSI", "R8 ", 
        "R9 ", "R10", "R11", "R12", "R13", "R14", "R15", "RIP",
        "CS ", "RFLAGS", "RSP", "SS "
    };
    
    // Imprimir registros en dos columnas
    for(int i = 0; i < CANT_REGS; i++) {
        textWrite(STDERR, " ", 1);
        textWrite(STDERR, regs[i], strlen(regs[i]));
        textWrite(STDERR, ": 0x", 4);
        uint8_t * hexValue = numToHex(registersArrayExceptions[i]);
        textWrite(STDERR, hexValue, 16);
        textWrite(STDERR, "\n", 1);
    }
}


// Orden: RAX, RBX, RCX, RDX, RBP, RDI, RSI, R8, R9, R10, R11, R12, R13, R14, R15, RIP, CS, RFLAGS, RSP, SS 
void exceptionDispatcher(uint64_t exception) {
    uint8_t * title = "";
    
    if (exception == ZERO_EXCEPTION_ID){
        title = "DIVISION BY ZERO EXCEPTION";
    }
    
    if(exception == OPCODE_EXCEPTION_ID){
        title = "INVALID OPCODE EXCEPTION";
    }

    Color backgroundColor = {0, 0, 0};
    Color textColor = {255, 0, 0};
    
    colorClearScreen(backgroundColor);
    setMode(TEXT_MODE, backgroundColor);
    
    printTitle(title);
    printRegs(title);
    
    textWrite(STDERR, "\n Press any key to continue...", 29);
    uint16_t buffer;

    picMasterMask(0xFD);                    // Solo habilitar interrupciones de teclado
    _sti();
    while(_read(STDIN, &buffer, 1) == 0);
    _cli();
    picMasterMask(MASTER_MASK);     

    // Limpiar pantalla al salir
    Color cleanColor = {0, 0, 0};
    colorClearScreen(cleanColor);
}