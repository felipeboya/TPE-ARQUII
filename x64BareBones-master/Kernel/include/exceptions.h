#ifndef EXCEPTIONS_H
#define EXCEPTIONS_H

#include <stdint.h>
#include <video.h>
#include <idtLoader.h>
#include <syscalls.h>
#include <status.h>

#define ZERO_EXCEPTION_ID 0
#define OPCODE_EXCEPTION_ID 6

#define CANT_REGS 20

void exceptionDispatcher(uint64_t exception);

#endif