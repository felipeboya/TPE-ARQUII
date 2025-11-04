#include "../include/time.h"

#define TICKS_PER_SEC 18

static uint64_t totalTicks = 0;

void timerHandler() {
	totalTicks++;
}

int ticksElapsed() {
	return totalTicks;
}

int secondsElapsed() {
	return totalTicks / TICKS_PER_SEC;
}

void sleepForTicks(uint64_t sleepTicks){
	uint64_t start = totalTicks;
	while(totalTicks - start < sleepTicks){
		_hlt();
	}
}