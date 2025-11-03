#ifndef LIBRARY_H
#define LIBRARY_H

#include <stdint.h>
#include <status.h>

#define TEXT_MODE 0
#define VIDEO_MODE 1

#define REGISTERS_QTY 20

typedef struct {
    uint64_t rax, rbx, rcx, rdx, rbp, rdi, rsi, r8, r9, r10, r11, r12, r13, r14, r15, rip, cs, rflags, rsp, ss;
} CpuSnapshot;

typedef CpuSnapshot * CpuSnapshotPtr;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

typedef struct {
    uint8_t year;
    uint8_t month;
    uint8_t day;
    uint8_t hour;
    uint8_t minutes;
    uint8_t seconds;
} timeStruct;

typedef timeStruct * timeStructPtr;

struct ScreenInformation{
    uint64_t width;
    uint64_t height;
};

typedef struct ScreenInformation * screenInfoPtr;

uint64_t read(uint64_t fd, char * buffer, uint64_t count);
uint64_t write(uint64_t fd, const char * s);
uint64_t snapshot(CpuSnapshotPtr snapshot);
uint64_t setFontSize(uint64_t size);
uint64_t clearScreen();
uint64_t putPixel(uint64_t x, uint64_t y, Color color);
uint64_t drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, uint64_t color);
uint64_t drawFont(uint64_t x, uint64_t y, uint64_t ch, uint64_t color, uint64_t size);
uint64_t setMode(uint64_t mode, uint64_t color);
uint64_t sleep(uint64_t sleepTicks);
uint64_t playSound(uint64_t frequency, uint64_t time);
uint64_t getTime(timeStructPtr time);
uint64_t getScreenInfo(screenInfoPtr screenInformation);


#endif