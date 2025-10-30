#ifndef _TIME_H_
#define _TIME_H_

#include <lib.h>

void timerHandler();
int ticksElapsed();
int secondsElapsed();
void sleepForTicks(uint64_t sleepTicks);

#endif