#include <exceptions.h>

static uint8_t * numToString(uint64_t num, uint64_t base); 
static void printRegs(uint8_t * message, uint8_t cantCharsMessage);

uint64_t registersArrayExceptions[CANT_REGS];

// Orden: RAX, RBX, RCX, RDX, RBP, RDI, RSI, R8, R9, R10, R11, R12, R13, R14, R15, RIP, CS, RFLAGS, RSP, SS 

void exceptionDispatcher(uint64_t exception) {

    uint8_t * message = "";
    uint8_t messageCant = 1;
	if (exception == ZERO_EXCEPTION_ID){
        message = "Zero div exception\n\n";
        messageCant = 20;
    }

	if(exception == OPCODE_EXCEPTION_ID){
        message = "Invalid opcode\n\n";
        messageCant= 16;
    }

    printRegs(message, messageCant);
    textWrite(STDERR, "\nPress any key to continue", 30);
    uint16_t buffer;

    picMasterMask(0xFD);                    //Solo habilito interrupciones de teclado
    _sti();
    while(_read(STDIN, &buffer,1) == 0);
    _cli();
    picMasterMask(MASTER_MASK);     

    Color c = {0, 0, 0};
    colorClearScreen(c);
}

static uint8_t * numToString(uint64_t num, uint64_t base) {
    static char buffer[64];
    uint8_t * ptr = &buffer[63];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while(num != 0);
    return ptr;
}

static void printRegs(uint8_t * message, uint8_t cantCharsMessage){
    uint8_t * regs[] = {"RAX", "RBX", "RCX", "RDX", "RBP", "RDI", "RSI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "CS", "RFLAGS", "RSP", "SS"};
    Color col = {255,255,255};
    colorClearScreen(col);
    setMode(TEXT_MODE, col);

    textWrite(STDERR, message, cantCharsMessage);
    uint8_t newline = '\n';
    for(int i=0; i<CANT_REGS;i++){
        textWrite(STDERR,regs[i], 10);
        textWrite(STDERR, ": 0x", 4);
        textWrite(STDERR, numToString(registersArrayExceptions[i],16), 10);
        textWrite(STDERR, "h", 1);
        textWrite(STDERR,&newline, 1);
    }
}