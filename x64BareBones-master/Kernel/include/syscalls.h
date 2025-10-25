#ifndef SYSCALLS_H
#define SYSCALLS_H

#include <stdint.h>

#define SYS_WRITE 0x4

uint64_t sys_write(uint64_t fd, const char * buf, uint64_t size);


#endif