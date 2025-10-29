#ifndef SNAPSHOT_H
#define SNAPSHOT_H

#include <stdint.h>

#define REGISTERS_QTY 20

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rbp, rdi, rsi, rsp, r8, r9, r10, r11, r12, r13, r14, r15, rip, cs, ,rflags, rsp, ss;
} CpuSnapshot;

typedef CpuSnapshot * CpuSnapshotPtr;

extern uint64_t registersArray[REGISTERS_QTY];
extern uint64_t registerArrayAux[REGISTERS_QTY];

#endif