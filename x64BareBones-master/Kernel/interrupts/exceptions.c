#include <stdint.h>
#include <exceptions.h>
#include <video.h>
#include <idtLoader.h>
#include <syscalls.h>

#define STDERR 1

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6

#define CANT_REGS 20
uint64_t registersArrayExceptions[CANT_REGS];

// Orden: RAX, RBX, RCX, RDX, RBP, RDI, RSI, R8, R9, R10, R11, R12, R13, R14, R15, RIP, CS, RFLAGS, RSP, SS 

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

static void printRegs(uint8_t * message, uint8_t cant_chars_message){
    uint8_t * regs[] = {"RAX", "RBX", "RCX", "RDX", "RBP", "RDI", "RSI", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "CS", "RFLAGS", "RSP", "SS"};
    Color col = {255,255,255};
    colorClearScreen(col);
    setMode(TEXT_MODE, col);
    setFontSize(2);

    textWrite(STDERR, message, cant_chars_message);
    uint8_t newline = '\n';
    for(int i=0; i<CANT_REGS;i++){
        textWrite(STDERR,regs[i], 10);
        textWrite(STDERR, ": 0x", 4);
        textWrite(STDERR, numToString(registersArrayExceptions[i],16), 10);
        textWrite(STDERR, "h", 1);
        textWrite(STDERR,&newline, 1);
    }
}

void exceptionDispatcher(uint64_t exception) {

    uint8_t * message = "";
    uint8_t message_cant = 1;
	if (exception == ZERO_EXCEPTION_ID){
        message = "Zero div exception\n\n";
        message_cant = 20;
    }

	if(exception == OPCODE_EXCEPTION_ID){
        message = "Invalid opcode\n\n";
        message_cant= 16;
    }

    printRegs(message, message_cant);
    textWrite(STDERR, "\nPress any key to continue", 30);
    uint16_t buffer;



    picMasterMask(0xFD);                    //Solo habilito interrupciones de teclado
    _sti();
    while(_read(1, &buffer,1) == 0);
    _cli();
    picMasterMask(MASTER_MASK);     // restores to default

    Color c = {0, 0, 0};
    colorClearScreen(c);
}