#include <audio.h>

extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);

void playSound(uint32_t f) {
    uint32_t div = PIT_FREQUENCY / f;

    outb(0x43, 0xB6); // Channel 2: 0x42, square wave generator
    outb(0x42, (uint8_t) (div & 0xFF));
    outb(0x42, (uint8_t) ((div >> 8) & 0xFF));

    uint8_t aux = inb(0x61);
    if (aux != (aux | 3)) {
        outb(0x61, aux | 3);
    }
    return;
    
}

void stopSound() {
    uint8_t aux = inb(0x61) & 0xFC;
    outb(0x61, aux);
    return;
}

void beep(uint64_t frequency, uint64_t ticks) {
	playSound(frequency);
	sleepForTicks(ticks);
	stopSound();
}