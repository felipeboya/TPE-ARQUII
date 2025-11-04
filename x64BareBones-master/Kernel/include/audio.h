#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include <stdint.h>
#include <time.h>

#define PIT_FREQUENCY 1193182

void playSound(uint32_t f);

void noSound();

void beep(uint64_t freq, uint64_t time);

#endif