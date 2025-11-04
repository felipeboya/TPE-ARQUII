#include <shell.h>

#define COMMANDS_QUANTITY 11

static uint64_t fontSize = 1;

Command commands[COMMANDS_QUANTITY] = {
    {"help", "Display this help message", helpFunction},
    {"clear", "Clear the screen", clearFunction},
    {"registers", "Show CPU registers. Remember to press ` beforehand", registersFunction},
    {"time", "Show current date and time", timeFunction},
    {"zoomIn", "Zoom in the screen", zoomInFunction},
    {"zoomOut", "Zoom out the screen", zoomOutFunction},
    {"tron", "Play tron", tronFunction},
    {"beep", "Make a sound", beepFunction},
    {"benchmark", "Run system benchmarks", benchmarkFunction},
    {"divZero", "Throw an exception (divide by zero)", divZeroFunction},
    {"invOpCode", "Throw an exception (invalide op code)", invOpCodeFunction}
};

void execute(const char * entry);
static void toUtcMinus3(timeStructPtr time);
static void extractCommand(const char * buffer, char * command);
static void newLine();

// Shell Functions

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
        }else if ( c == KC_BACKSP ){
            continue;
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
}

void execute(const char * entry){
    for(uint64_t i = 0; i < COMMANDS_QUANTITY; i++){
        if( strcmp(commands[i].name, entry) == OK ){
            commands[i].function();
            return;
        }
    }
    printf("Shell: Command not found\n");
}

// Command Functions
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
    printSnapshot();
}

void timeFunction(){
    timeStruct time;
    getTime(&time);
    toUtcMinus3(&time);
    printf("Argentina\n");
    printf("%d/%d/%d\n", time.day, time.month, time.year);
    int64_t h = time.hour;
    printf("%d:%d:%d\n", h, time.minutes, time.seconds);
}

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
    playTron();
}

void beepFunction(){
    beep(440, 5);
}

void benchmarkFunction(){
    runBenchmarks();
}

void divZeroFunction(){
    div0();
}

void invOpCodeFunction(){
    opCode();
}

char getChar(){
    char c;
    while ( read(STDIN, &c, 1) == 0 || c > MAX_ASCII ){
        _hlt();
    }
    return (char) c;
}

// Static Functions
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

static void toUtcMinus3(timeStructPtr time){
    if (time->hour < 3){
        time->hour += 21;
        time->day--;
        if (time->day == 0){
            time->month--;
            if (time->month == 0){
                time->month = 12;
                time->year--;
            }
            if(time->month == 2){
                time->day = 28;
                if(time->year % 4 == 0){
                    time->day = 29;
                }
            } else if(time->month == 4 || time->month == 6 || time->month == 9 || time->month == 11){
                time->day = 30;
            } else{
                time->day = 31;
            }
        }
    }
    else{
        time->hour = time->hour - 3;
    }
}