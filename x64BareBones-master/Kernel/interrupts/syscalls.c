#include <syscalls.h>

#define REGISTERS_QTY 20

#define castToColor(c) ((Color){ (uint8_t)(((uint64_t)(c) >> 16) & 0xFF), (uint8_t)(((uint64_t)(c) >> 8)  & 0xFF), (uint8_t)((uint64_t)(c) & 0xFF) })
    
uint64_t registersArray[REGISTERS_QTY];
uint64_t registersArrayAux[REGISTERS_QTY];

uint64_t _read(uint64_t fd, char * buffer, uint64_t amount);
static uint64_t _textWrite(uint64_t fd, const char * buffer, uint64_t amount);
static uint64_t _registersSnapshot(CpuSnapshotPtr snapshot);
static uint64_t _setFontSize(uint64_t size);
static uint64_t _clearScreen();
static uint64_t _putPixel(uint64_t x, uint64_t y, uint64_t color);
static uint64_t _drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color);
static uint64_t _drawFont(uint64_t x, uint64_t y, uint64_t ch, uint64_t color, uint64_t size);
static uint64_t _setMode(uint64_t mode, uint64_t color);
static uint64_t _sleep(uint64_t sleepTicks);
static uint64_t _playSound(uint64_t frequency, uint64_t time);
static uint64_t _getTime(timeStructPtr time);
static uint64_t _getScreenInfo(screenInfoPtr screenInformation);
static uint64_t _drawLine(uint64_t x, uint64_t y, uint64_t width, uint64_t color);

uint64_t syscallDispatcher(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6){
    switch(syscall_id){
        case SYS_READ : 
            return _read(arg1, (char *) arg2, arg3);
        case SYS_WRITE : 
            return _textWrite(arg1, (const char *) arg2, arg3);
        case SYS_REGISTERS_SNAPSHOT :
            return _registersSnapshot((CpuSnapshotPtr) arg1);
        case SYS_SET_FONT_SIZE :
            return _setFontSize(arg1);
        case SYS_CLEAR_SCREEN :
            return _clearScreen();
        case SYS_PUT_PIXEL :
            return _putPixel(arg1, arg2, arg3);
        case SYS_DRAW_RECTANGLE :
            return _drawRectangle(arg1, arg2, arg3, arg4, arg5);
        case SYS_DRAW_FONT :
            return _drawFont(arg1, arg2, arg3, arg4, arg5);
        case SYS_SET_MODE :
            return _setMode(arg1, arg2);
        case SYS_SLEEP :
            return _sleep(arg1);
        case SYS_PLAY_SOUND :
            return _playSound(arg1, arg2);
        case SYS_GET_TIME :
            return _getTime((timeStructPtr) arg1);
        case SYS_GET_SCREEN_INFO :
            return _getScreenInfo((screenInfoPtr) arg1);
        case SYS_DRAW_LINE : 
            return _drawLine(arg1, arg2, arg3, arg4);
        default :
            return ERROR;
    }
}

// Lee el buffer cargado por el driver de teclado (keysBuffer)
uint64_t _read(uint64_t fd, char * buffer, uint64_t amount){
    if ( fd != STDIN ){
        return ERROR;
    }

    uint64_t i = 0;
    uint8_t c;
    while (i < amount && (c = getChar()) != 0){
        buffer[i++] = c;
    }

    return i;
}

static uint64_t _textWrite(uint64_t fd, const char * buffer, uint64_t amount){
    return textWrite(fd, buffer, amount);
}

static uint64_t _registersSnapshot(CpuSnapshotPtr snapshot){
    if ( snapshot == NULL ){
        return ERROR;
    }
    
    snapshot->rax    = registersArray[0];
    snapshot->rbx    = registersArray[1];
    snapshot->rcx    = registersArray[2];
    snapshot->rdx    = registersArray[3];
    snapshot->rbp    = registersArray[4];
    snapshot->rdi    = registersArray[5];
    snapshot->rsi    = registersArray[6];
    snapshot->r8     = registersArray[7];
    snapshot->r9     = registersArray[8];
    snapshot->r10    = registersArray[9];
    snapshot->r11    = registersArray[10];
    snapshot->r12    = registersArray[11];
    snapshot->r13    = registersArray[12];
    snapshot->r14    = registersArray[13];
    snapshot->r15    = registersArray[14];
    snapshot->rip    = registersArray[15];
    snapshot->cs     = registersArray[16];
    snapshot->rflags = registersArray[17];
    snapshot->rsp    = registersArray[18];
    snapshot->ss     = registersArray[19];

    return OK;
}

static uint64_t _setFontSize(uint64_t size){
    return setFontSize(size);
}

static uint64_t _clearScreen(){
    Color color = {0, 0, 0};
    return colorClearScreen(color);
}

static uint64_t _putPixel(uint64_t x, uint64_t y, uint64_t color){
    return putPixel(x, y, castToColor(color));
}

static uint64_t _drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color){
    return drawRectangle(x, y, width, height, castToColor(color));
}

static uint64_t _drawFont(uint64_t x, uint64_t y, uint64_t ch, uint64_t color, uint64_t size){
    return drawFont(x, y, ch, castToColor(color), size);
}

static uint64_t _setMode(uint64_t mode, uint64_t color){
    return setMode(mode, castToColor(color));
}

static uint64_t _sleep(uint64_t sleepTicks){
    sleepForTicks(sleepTicks);
    return OK;
}

static uint64_t _playSound(uint64_t frequency, uint64_t time){
    beep(frequency, time);
    return OK;
}

static uint64_t _getTime(timeStructPtr time){
    time->seconds = getRTCSeconds();
    time->minutes =  getRTCMinutes();
    time->hour =  getRTCHours();
    time->day = getRTCDayOfMonth();
    time->month = getRTCMonth();
    time->year = getRTCYear();
    return OK;
}

static uint64_t _getScreenInfo(screenInfoPtr screenInformation){
    return getScreenInfo(screenInformation);
}

static uint64_t _drawLine(uint64_t x, uint64_t y, uint64_t width, uint64_t color){
    return drawLine(x, y, width, castToColor(color));
}

void snapshot(void){
    for ( int i = 0; i < REGISTERS_QTY; i++ ){
        registersArray[i] = registersArrayAux[i];
    }
}