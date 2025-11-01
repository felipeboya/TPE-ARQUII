#ifndef VIDEODRIVER_H
#define VIDEODRIVER_H

#include <stdint.h>
#include <status.h>

#define TEXT_MODE 0
#define VIDEO_MODE 1

/**
 * Structure representing the VBE mode information.
 */
struct vbeModeInfoStructure {
    uint16_t attributes;	// deprecated, only bit 7 should be of interest to you, and it indicates the mode supports a linear frame buffer.
    uint8_t windowA;		// deprecated
    uint8_t windowB;		// deprecated
    uint16_t granularity;	// deprecated; used while calculating bank numbers
    uint16_t windowSize;
    uint16_t segmentA;
    uint16_t segmentB;
    uint32_t winFuncPtr;	// deprecated; used to switch banks from protected mode without returning to real mode
    uint16_t pitch;			// number of bytes per horizontal line
    uint16_t width;			// width in pixels
    uint16_t height;		// height in pixels
    uint8_t wChar;			// unused...
    uint8_t yChar;			// ...
    uint8_t planes;
    uint8_t bpp;			// bits per pixel in this mode
    uint8_t banks;			// deprecated; total number of banks in this mode
    uint8_t memoryModel;
    uint8_t bankSize;		// deprecated; size of a bank, almost always 64 KB but may be 16 KB...
    uint8_t imagePages;
    uint8_t reserved0;

    uint8_t redMask;
    uint8_t redPosition;
    uint8_t greenMask;
    uint8_t greenPosition;
    uint8_t blueMask;
    uint8_t bluePosition;
    uint8_t reservedMask;
    uint8_t reservedPosition;
    uint8_t directColorAttributes;

    uint32_t framebuffer;		// physical address of the linear frame buffer; write here to draw to the screen
    uint32_t offScreenMemOff;
    uint16_t offScreenMemSize;	// size of memory in the framebuffer but not being displayed on the screen
    uint8_t reserved1[206];
} __attribute__ ((packed));

typedef struct vbeModeInfoStructure * vbeInfoPtr;

typedef struct {
    uint8_t r;
    uint8_t g;
    uint8_t b;
} Color;

// Dimensiones de la pantalla
struct ScreenInformation{
    uint64_t width;
    uint64_t height;
};

typedef struct ScreenInformation * screenInfoPtr;

uint64_t putPixel(uint64_t x, uint64_t y, Color color);
uint64_t drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, Color color);
uint64_t drawFont(uint64_t x, uint64_t y, uint64_t ch, Color color, uint64_t size);
uint64_t setFontSize(uint64_t size);
void setFontColor(Color c);
uint64_t textWrite(uint64_t fd, const char * buffer, int64_t amount);
uint64_t getScreenInfo(screenInfoPtr screenInformation);
uint64_t setMode(uint64_t mode, Color c);
uint64_t colorClearScreen(Color color);

#endif