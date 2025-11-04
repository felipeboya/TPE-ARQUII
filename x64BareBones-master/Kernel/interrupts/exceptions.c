#include <stdint.h>
#include <interrupts.h>
#include <syscalls.h>
#include <video.h>

#define ZERO_EXCEPTION_ID 0
#define SIX_EXCEPTION_ID 6
#define ZERO_EXCEPTION_MESSAGE "Division Error.\n"
#define SIX_EXCEPTION_MESSAGE "Invalid Opcode.\n"

extern void picMasterMask(uint16_t mask);
static void zeroDivision();
static void printRegs();
static void printExceptionMessage();
static uint64_t strlen(char* s);
static char * numToString(uint64_t num, uint64_t base);
static void emptyKeyboardBuffer();

void exceptionDispatcher(uint64_t exception) {
	Color color = {0, 0, 0};
    colorClearScreen(color);
	printRegs();

	if (exception == ZERO_EXCEPTION_ID){
		printExceptionMessage(ZERO_EXCEPTION_MESSAGE);
	}else if(exception == SIX_EXCEPTION_ID){
		printExceptionMessage(SIX_EXCEPTION_MESSAGE);
	}

	const char * next = "\nPress any letter key to return to shell\n";
    textWrite(1, next, strlen(next));

    picMasterMask(0xFD);    // Solo IRQ1 (teclado)
    _sti();
    emptyKeyboardBuffer();
    getKey();           // se bloquea c/ hlt hasta recibir un byte
    _cli();
    picMasterMask(0xFC);
	emptyKeyboardBuffer();
    colorClearScreen(color);
	return;
}

static void zeroDivision() {
	// Handler para manejar excepcíon
}

static void printRegs(){
	char * regs[] = {"RAX", "RBX", "RCX", "RDX", "RSI", "RDI", "RBP", "R8", "R9", "R10", "R11", "R12", "R13", "R14", "R15", "RIP", "CS", "RFLAGS", "RSP", "SS"};

	const char newline = '\n';

	for(int i=0; i<REGISTERS_QTY; i++){
		textWrite(1, regs[i], strlen(regs[i]));
		textWrite(1, ": 0x", 4);
		char * aux = numToString(registersArrayException[i], 16);
		textWrite(1, aux, strlen(aux));
		textWrite(1, newline, 1);
	}

	return;
}

static void printExceptionMessage(char* message){
	textWrite(1, message, strlen(message));
	return;
}

static uint64_t strlen(char* s){
	char * p = s;
	while(*p){
		p++;
	}
	return (p - s);
}

static char * numToString(uint64_t num, uint64_t base) {
    static char buffer[64];
    char * ptr = &buffer[63];
    *ptr = '\0';
    do {
        *--ptr = "0123456789abcdef"[num % base];
        num /= base;
    } while(num != 0);
    return ptr;
}

static void emptyKeyboardBuffer(){
    uint8_t c;
    while ((c = getChar()) != 0);
	return;
}