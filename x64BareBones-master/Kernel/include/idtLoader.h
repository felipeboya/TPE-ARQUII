#ifndef _IDTLOADER_H_
#define _IDTLOADER_H_

#include <stdint.h>
#include <defs.h>
#include <interrupts.h>

#define MASTER_MASK 0xFC
#define SLAVE_MASK 0xFF

void load_idt();

#endif 
