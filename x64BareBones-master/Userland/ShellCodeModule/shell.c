#include <shell.h>
#include <library.h>
#include <utils.h>

#define COMMANDS_QUANTITY 8

Command commands[COMMANDS_QUANTITY] = {
    {"help" , "- Display this help message", helpFunction},
    {"clear", "- Clear the screen", clearScreen},
    {"registers", "- Show CPU registers. Remember to press F1 beforehand", registersFunction},
    {"time", "- Show current date and time", timeFunction},
    {"zoomIn", "- Zoom in the screen", zommInFunction},
    {"zoomOut", "- Zoom out the screen", zoomOutFunction},
    {"tron", "- Play tron", tronFunction},
    {"beep", "- Make a sound", beepFunction},
};

static char getChar();
static void extractCommand(const char * buffer, char * command);

void printPrompt(){
    char prompt[] = "$> ";  
    write(STDOUT, prompt);
}

void readLine(char * buffer){
    uint64_t bufSize = BUFF_SIZE;   // (idealmente usariamos malloc)
    uint64_t bufIdx = 0;
    char c;

    while(1){
        c = getChar();
        if ( c == KC_ENTER || bufIdx >= bufSize - 1){
            buffer[bufIdx] = 0;
            return;
        }

        if ( c != KC_BACKSP ){
            buffer[bufIdx++] = c;
        }
        char letter[2] = {c, 0};
        write(STDOUT, letter);
    }
}

void processLine(const char * buffer){
    char command[BUFF_SIZE]; 
    extractCommand(buffer, command);
    write(STDOUT, "\n");
    execute(command);
    write(STDOUT, "\n");
}

void execute(const char * entry){
    for(uint64_t i = 0; i < COMMANDS_QUANTITY; i++){
        if( strcmp(commands[i].name, entry) == OK ){
            commands[i].function();
            return;
        }
    }
    write(STDOUT, "Shell: Command not found");
}

// Commands
void helpFunction(){
    write(STDOUT, "Hola, estoy en el comando");
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

//  NO ANDA
void beepFunction(){
    playSound(440, 20);
}

//Static

static char getChar(){
    char c;
    while ( read(STDIN, &c, 1) == 0 || c > MAX_ASCII ){
        _hlt();
    }
    return (char) c;
}

static void extractCommand(const char * buffer, char * command){
    uint64_t cmdIdx = 0;
    for ( int i = 0; i < BUFF_SIZE; i++ ){
        if (buffer[i] == ' ' || buffer[i] == 0){
            return;
        }
        command[cmdIdx++] = buffer[i];
    }
}
