#include <syscalls.h>
#include <status.h>
#include <video.h>
#include <keyboard.h>

static uint64_t _read(uint64_t fd, char * buffer, uint64_t amount);
static uint64_t _textWrite(uint64_t fd, const char * buffer, uint64_t amount);

uint64_t syscallDispatcher(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6){
    switch(syscall_id){
        case SYS_READ : 
            return _read(arg1, (char *) arg2, arg3);
        case SYS_WRITE : 
            return _textWrite(arg1, (const char *) arg2, arg3);
        default :
            return ERROR;
    }
}

// Lee el buffer cargado por el driver de teclado (keysBuffer)
static uint64_t _read(uint64_t fd, char * buffer, uint64_t amount){
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