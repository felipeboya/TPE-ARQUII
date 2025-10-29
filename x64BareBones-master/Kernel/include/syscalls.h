#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

// System Calls: https://syscalls64.paolostivanin.com/

#define SYS_READ 0x0
#define SYS_WRITE 0x1
#define SYS_REGISTERS_SNAPSHOT 0x2
#define SYS_SET_FONT_SIZE 0x3
#define SYS_CLEAR_SCREEN 0x4
#define SYS_PUT_PIXEL 0x5
#define SYS_DRAW_RECTANGLE 0x6
#define SYS_DRAW_FONT 0x7
#define SYS_SET_MODE 0x8


uint64_t syscallDispatcher(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif