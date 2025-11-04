#include <tron.h>

char map[MAP_HEIGHT][MAP_WIDTH];

static void welcome();
static void initializeMap();
static void singlePlayer();
static void retryMenuSingleplayer();
static void multiPlayer();
static void retryMenuMultiplayer(int whoWon, int scores[]);
static void get_player_1_direction(uint16_t c, int64_t * direction);
static void get_player_2_direction(uint16_t c, int64_t * direction);
static void playArcadeSong();
static void printCenteredString(uint64_t x, uint64_t y, const char * str, uint64_t font_size, uint64_t color);
static void printString(uint64_t x, uint64_t y, const char * str, uint64_t font_size, uint64_t color);
static void fillPosition(uint64_t x, uint64_t y, uint64_t color);

static uint64_t level = 0;
static const uint64_t levels[] = {LEVEL_0, LEVEL_1, LEVEL_2, LEVEL_3};
static singlePlayerPoints = 0;

void playTron(){
    setMode(VIDEO_MODE, 0);
    welcome();
    clearScreen();
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"Saliendo!",3, GREEN);
    sleep(20);
    beep(100,5);
    beep(50,5);
    setMode(TEXT_MODE, 0);
    return;
}

void welcome(){
    clearScreen();
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"Bienvenido al Tron!",3, GREEN);
    sleep(20);
    playArcadeSong();
    clearScreen();

    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-100,"Elija el modo de juego: (Presione la tecla correspondiente)",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-50,"s. Single Player",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"m. Multiplayer",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+50,"q. Exit",2, GREEN);
    char c;
    while((( c = getChar() ) != 'q' ) && (c != 'm' ) && (c != 's' )){}
    
    if (c != 'q'){
        clearScreen();
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-100,"Elija el nivel: (Presione la tecla correspondiente)",2, GREEN);
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-50,"0. Facil",2, GREEN);
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"1. Medio",2, GREEN);
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+50,"2. Dificil",2, GREEN);
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+100,"3. Imposible",2, GREEN);
        
        char lev;
        while((( lev = getChar() ) != '0' ) && (lev != '1' ) && (lev != '2' ) && (lev != '3' )){}
        level = lev - '0';
        clearScreen();
    }
    
    switch (c){
        case 'q': break;
        case 's': singlePlayer(); break;
        case 'm': multiPlayer(); break;
    }
    return;
}

static void singlePlayer(){
    singlePlayerPoints = 0;
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"Utilice W-A-S-D para moverse",2, GREEN);
    sleep(50);
    clearScreen();

    initializeMap();
    uint64_t p1x = PLAYER_1_INIT_X;
    uint64_t p1y = PLAYER_1_INIT_Y - 10;
    uint64_t p1_lost = 0;
    int64_t direction[2] = {0,-1}; // al principio va haci­a arriba
    uint16_t buffer[10];
    uint64_t buffer_size = 0;

    fillPosition(p1x, p1y, BLUE);

    while(1){
        singlePlayerPoints++;
        sleep(1);
        buffer_size = 0;
        buffer_size = read(STDIN, buffer, 1);

        if(buffer_size > 0){
            get_player_1_direction(buffer[buffer_size - 1], direction);
        }
        p1x += direction[0];
        p1y += direction[1];
        if(map[p1y][p1x] == 1){
            p1_lost = 1;
            break;
        }
        fillPosition(p1x, p1y, BLUE);

    }
    retryMenuSingleplayer();
}

static void retryMenuSingleplayer(){
    clearScreen();
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-150,"GAME OVER",3, RED);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-50,"Tu puntaje fue",3, BLUE);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,numToString(singlePlayerPoints,10),2, BLUE);
    sleep(40);
    clearScreen();
    printCenteredString(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2,"r. Retry",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2+100, SCREEN_HEIGHT/2,"m. Main Menu",2, GREEN);

    char c;
    while((( c = getChar() ) != 'r' ) && (c != 'm' )){}
    clearScreen();
    switch (c){
        case 'r': singlePlayer(); break;
        case 'm': welcome(); break;
    }
    sleep(50);
}

static void retryMenuMultiplayer(int whoWon, int scores[]){
    clearScreen();
    if(whoWon == 1)
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"Ganador: Jugador 1",3, BLUE);
    else if(whoWon == 2)
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"Ganador: Jugador 2",3, ORANGE);
    else
        printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2,"Empate",3, GREEN);

    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 50,"Puntaje",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2 + 100,numToString(scores[0],10),2, GREEN);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2 + 100," VS ",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2+100, SCREEN_HEIGHT/2 + 100,numToString(scores[1],10),2, GREEN);

    sleep(50);
    clearScreen();
    printCenteredString(SCREEN_WIDTH/2-100, SCREEN_HEIGHT/2,"r. Retry",2, GREEN);
    printCenteredString(SCREEN_WIDTH/2+100, SCREEN_HEIGHT/2,"m. Main Menu",2, GREEN);
    
    char c;
    while((( c = getChar() ) != 'r' ) && (c != 'm' )){}
    clearScreen();
    switch (c){
        case 'r': multiPlayer(); break;
        case 'm': welcome(); break;
    }
}

static void multiPlayer(){
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2-50,"Jugador 1: Utilice W-A-S-D para moverse",2, BLUE);
    printCenteredString(SCREEN_WIDTH/2, SCREEN_HEIGHT/2+50,"Jugador 2: Utilice I-J-K-L para moverse",2, ORANGE);
    sleep(50);
    clearScreen();
    static int scores[2] = {0,0}; //No Funciona Si Jugas mas de 2^64 Veces jaja :D
    initializeMap();
    uint64_t p1x = PLAYER_1_INIT_X;
    uint64_t p1y = PLAYER_1_INIT_Y - 1;
    uint64_t p2x = PLAYER_1_INIT_X;
    uint64_t p2y = 1;

    uint64_t p1_lost = 0;
    uint64_t p2_lost = 0;

    int64_t p1dir[2] = {0,-1};
    int64_t p2dir[2] = {0, 1};

    uint16_t buffer[10];
    uint64_t buffer_size = 0;

    fillPosition(p1x, p1y, BLUE);
    fillPosition(p2x, p2y, ORANGE);

    while(1){
        sleep(1);
        buffer_size = 0;
        buffer_size = read(0, buffer, 1);

        if(buffer_size > 0){
            get_player_1_direction(buffer[buffer_size - 1], p1dir);
            get_player_2_direction(buffer[buffer_size - 1], p2dir);

        }
        p1x += p1dir[0];
        p1y += p1dir[1];
        p2x += p2dir[0];
        p2y += p2dir[1];
        if(p1y==p2y && p1x==p2x){
            p1_lost = 1;
            p2_lost = 1;
            break;
        }
        if(map[p1y][p1x] == 1){
            p1_lost = 1;
        }
        if(map[p2y][p2x] == 1){
            p2_lost = 1;
        }
        if(p1_lost || p2_lost){
            break;
        }
        fillPosition(p1x, p1y, BLUE);
        fillPosition(p2x, p2y, ORANGE);

    }
    if(p1_lost && p2_lost){
        retryMenuMultiplayer(0,scores);
    }
    if(p2_lost){
        scores[0]++;
        retryMenuMultiplayer(1,scores);
    } else {
        scores[1]++;
        retryMenuMultiplayer(2,scores);
    }
}

static void get_player_1_direction(uint16_t c, int64_t * direction){
    switch(c) {
        case ('w'):
            if(direction[1] == 1){
                break;
            }
            direction[0] = 0;
            direction[1] = -1;
            break;
        case ('a'):
            if(direction[0] == 1){
                break;
            }
            direction[0] = -1;
            direction[1] = 0;
            break;
        case ('s'):
            if(direction[1] == -1){
                break;
            }
            direction[0] = 0;
            direction[1] = 1;
            break;
        case ('d'):
            if(direction[0] == -1){
                break;
            }
            direction[0] = 1;
            direction[1] = 0;
            break;
    }
}

static void get_player_2_direction(uint16_t c, int64_t * direction){
    switch(c) {
        case ('i'):
            if(direction[1] == 1){
                break;
            }
            direction[0] = 0;
            direction[1] = -1;
            break;
        case ('j'):
            if(direction[0] == 1){
                break;
            }
            direction[0] = -1;
            direction[1] = 0;
            break;
        case ('k'):
            if(direction[1] == -1){
                break;
            }
            direction[0] = 0;
            direction[1] = 1;
            break;
        case ('l'):
            if(direction[0] == -1){
                break;
            }
            direction[0] = 1;
            direction[1] = 0;
            break;
    }
}

static void printCenteredString(uint64_t x, uint64_t y, const char * str, uint64_t font_size, uint64_t color) {    
    uint64_t len = strlen(str);
    uint64_t start_x = x - (len / 2) * CHARACTER_WIDTH * font_size;
    uint64_t start_y = y;
    printString(start_x, start_y, str, font_size, color);
}

static void printString(uint64_t x, uint64_t y, const char * str, uint64_t font_size, uint64_t color) {
    for (uint64_t i = 0; str[i] != '\0'; i++) {
        drawFont(x + i * CHARACTER_WIDTH * font_size, y, str[i], color, font_size);
    }
}

static void playArcadeSong() {
    // Frequencies for the notes in the song.
    uint32_t C4 = 262;
    uint32_t D4 = 294;
    uint32_t E4 = 330;
    uint32_t F4 = 349;
    uint32_t G4 = 392;
    uint32_t A4 = 440;
    uint32_t B4 = 494;
    uint32_t C5 = 523;

    // Duration of each note in milliseconds.
    int quarter_note = 500 / 55;
    int eighth_note = quarter_note / 3;

    // Play the song.
    beep(C4, eighth_note);
    beep(E4, eighth_note);
    beep(G4, eighth_note);
    beep(C5, eighth_note);
}

static void initializeMap(){
    for(int i = 0; i < MAP_HEIGHT; i++){
        for(int j = 0; j < MAP_WIDTH; j++){
            if(i == 0 || i == MAP_HEIGHT-1 || j == 0 || j == MAP_WIDTH-1){
                fillPosition(j, i, GRAY);
            }else if ( i % levels[level] == 0 && j % levels[level] == 0){
                fillPosition(j, i, GRAY);
            }else{
                map[i][j] = 0;
            }
        }
    }
    return;
}

static void fillPosition(uint64_t x, uint64_t y, uint64_t color){
    if(x < 0 || y < 0 || x >= MAP_WIDTH || y > MAP_HEIGHT){
        return;
    }
    drawRectangle(x*PIXEL, y*PIXEL, PIXEL, PIXEL, color);
    map[y][x] = 1;
    return;
}