//This code was based off the following website: https://wiki.osdev.org/PC_Speaker

//IMPORTANT: This will work on real hardware and in QEMU if it started with -audiodev pa,id=speaker -machine pcspk-audiodev=speaker
// macOS: qemu-system-x86_64 -hda Image/x64BareBonesImage.qcow2 -m 512 -audiodev coreaudio,id=snd0 -machine pcspk-audiodev=snd0

#include <audio.h>

extern void outb(uint16_t port, uint8_t value);
extern uint8_t inb(uint16_t port);

void playSound(uint32_t f) {
    uint32_t div = PIT_FREQUENCY / f;

    outb(0x43, 0xB6); // Channel 2: 0x42, square wave generator
    outb(0x42, (uint8_t) (div & 0xFF));
    outb(0x42, (uint8_t) ((div >> 8) & 0xFF));

    uint8_t tmp = inb(0x61);
    if (tmp != (tmp | 3)) {
        outb(0x61, tmp | 3);
    }
    return;   
}

void noSound() {
    uint8_t tmp = inb(0x61) & 0xFC;
    outb(0x61, tmp);
    return;
}

void beep(uint64_t frequency, uint64_t ticks) {
	playSound(frequency);
	sleepForTicks(ticks);
	noSound();
}