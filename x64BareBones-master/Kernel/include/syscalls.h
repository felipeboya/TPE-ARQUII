#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

// System Calls: https://syscalls64.paolostivanin.com/

#define SYS_WRITE 0x1

uint64_t syscallDispatcher(uint64_t syscall_id, uint64_t arg1, uint64_t arg2, uint64_t arg3, uint64_t arg4, uint64_t arg5, uint64_t arg6);

#endif