#ifndef AUDIO_DRIVER_H
#define AUDIO_DRIVER_H

#include <stdint.h>

#define PIT_FREQUENCY 1193182

void playSound(uint32_t f);

void stopSound();

void beep(uint64_t freq, uint64_t time);

#endif