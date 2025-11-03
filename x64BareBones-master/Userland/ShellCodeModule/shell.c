#include <shell.h>
#include <library.h>
#include <utils.h>

#define COMMANDS_QUANTITY 8

static uint64_t fontSize = 1;

Command commands[COMMANDS_QUANTITY] = {
    {"help", "Display this help message", helpFunction},
    {"clear", "Clear the screen", clearFunction},
    {"registers", "Show CPU registers. Remember to press F1 beforehand", registersFunction},
    {"time", "Show current date and time", timeFunction},
    {"zoomIn", "Zoom in the screen", zoomInFunction},
    {"zoomOut", "Zoom out the screen", zoomOutFunction},
    {"tron", "Play tron", tronFunction},
    {"beep", "Make a sound", beepFunction},
};

static char getChar();
static void extractCommand(const char * buffer, char * command);
static void newLine();

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

        char letter[2] = {c, 0};    // null terminated
        if( c == KC_BACKSP && bufIdx > 0 ){
            bufIdx--;
        }

        if ( c != KC_BACKSP ){
            buffer[bufIdx++] = c;
        }
        write(STDOUT, letter);
    }
}

void processLine(const char * buffer){
    char command[BUFF_SIZE]; 
    extractCommand(buffer, command);
    newLine();
    execute(command);
    newLine();
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
    write(STDOUT, "Use any of the following commands: \n");
    for(uint64_t i = 0; i < COMMANDS_QUANTITY; i++){
        write(STDOUT, " - ");
        write(STDOUT, commands[i].name);
        write(STDOUT, ": ");
        write(STDOUT, commands[i].description);
        newLine();
    }
}

void clearFunction(){
    clearScreen();
}

void registersFunction(){
}

void timeFunction(){

}

// Faltan syscalls de zoomIn y zoomOut
void zoomInFunction(){
    fontSize++;
    setFontSize(fontSize);
}

void zoomOutFunction(){
    if (fontSize <= 1){
        write(STDERR, "Unable to execute command: Minimum size displayed");
        return;
    }
    fontSize--;
    setFontSize(fontSize);
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
            break;
        }
        command[cmdIdx++] = buffer[i];
    }

    command[cmdIdx] = 0; 
}

static void newLine(){
    write(STDOUT, "\n");
}