#ifndef SHELL_H
#define SHELL_H

#include <library.h>
#include <utils.h>

#define BUFF_SIZE  125
#define MAX_ASCII 255

#define KC_ENTER  '\n'
#define KC_BACKSP '\b'
#define KC_TAB    '\t'
#define KC_ESC    0x1B
#define KC_SPACE  ' '

typedef struct {
    char * name;
    char * description;
    void (*function)(void);
} Command;

void printPrompt();
void readLine(char * buffer);
void processLine(const char * buffer);

void helpFunction();
void clearFunction();
void registersFunction();
void timeFunction();
void zoomInFunction();
void zoomOutFunction();
void tronFunction();
void beepFunction();

#endif