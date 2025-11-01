#include <shell.h>
#include <library.h>

#define COMMANDS_QUANTITY 8


Command commands[COMMANDS_QUANTITY] = {
    {"help" , "- Display this help message", helpFunction},
    {"clear", "- Clear the screen", clearScreen},
    {"registers", "- Show CPU registers. Remember to press F1 beforehand", registersFunction},
    {"time", "- Show current date and time", timeFunction},
    {"zoomIn", "- Zoom in the screen", zommInFunction},
    {"zoomOut", "- Zoom out the screen", zoomOutFunction},
    {"tron", "- Play tron", tronFunction},
    {"beep", "- Make a sound", playSound},
};

void printPrompt(){
    char prompt[] = {'$', '>', ' '}; 
    write(STDOUT, prompt);
}

// Commands
void helpFunction(){
    
}

void registersFunction(){
}

void timeFunction(){

}

// Faltan syscalls de zoomIn y zoomOut
void zommInFunction(){

}

void zoomOutFunction(){

}

void tronFunction(){

}

void beepFunction(){

}