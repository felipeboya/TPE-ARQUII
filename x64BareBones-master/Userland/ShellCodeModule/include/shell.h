#ifndef SHELL_H
#define SHELL_H

typedef struct {
    char * name;
    char * description;
    void (*function)(void *);
} Command;

void printPrompt();

void helpFunction();
void registersFunction();
void timeFunction();
void clearFunction();
void zommInFunction();
void zoomOutFunction();
void tronFunction();
void beepFunction();


#endif