#include <video.h>
#include <font.h>

#define SCREEN_WIDTH vbeModeInfo->width
#define SCREEN_HEIGHT vbeModeInfo->height

#define CHAR_BUFFER_ROWS  48         // Numero de caracteres disponibles con la fuente en 1 (en x).
#define CHAR_BUFFER_COLS  128        // Numero de caracteres disponibles con la fuente en 1 (en y).
#define SPACE_PER_TAB 4
#define Y_FONT_OFFSET (FONT_HEIGHT * fontSize)
#define X_FONT_OFFSET (FONT_WIDTH * fontSize)
#define ROWS_TO_REBUFFER(rows_in_screen) (((rows_in_screen)/(2))+1)     // Hace que el reBuffer me imprima la ultima mitad de la pantalla

typedef struct{
    uint64_t x;
    uint64_t y;
}Point;

typedef struct{
    uint8_t c;			// Caracter ASCII
    uint8_t fd;			// File descriptor
}charBufferType;

static vbeInfoPtr vbeModeInfo = (vbeInfoPtr) 0x0000000000005C00;
extern uint8_t fontBitmap[4096];
static Color backgroundColor = {0,0,0}; 	// Color default del fondo
static Color fontColor = {255,255,255};	    // Color default de la fuente
static Color stderrColor = {255, 0,0};
static uint64_t fontSize = 1;				// Tamaño base de fuente
static uint8_t driverMode = TEXT_MODE;		// Habria que inicializar en VIDEO_MODE?
static Point currentScreenPoint = {0,0};
static charBufferType charBuffer[CHAR_BUFFER_ROWS * CHAR_BUFFER_COLS]; // Array de caracteres en pantalla
static uint64_t bufferIndex = 0;

static uint8_t overrideMode = 0;
static uint64_t charBufferRowsZoomed = CHAR_BUFFER_ROWS;
static uint64_t charBufferColsZoomed = CHAR_BUFFER_COLS;

static uint64_t inTextMode();
static uint64_t inVideoMode();
static void addCharToBuffer(uint8_t c, uint8_t fd);
static void printFont(charBufferType letter);
static void clearScreen();
static void newLinePrint();
static void newLineBuff();
static void newLine();
static void backSpaceBuffer();
static void backSpace();
static void backSpacePrint();
static void tabulator();
static void reBufferPrint();
static void printBuffer();

int64_t putPixel(uint64_t x, uint64_t y, Color color){
	if(x >= SCREEN_WIDTH || y >= SCREEN_HEIGHT){
		return ERROR;
	}
	if(!inVideoMode()){
		return ERROR;
	}
	uint8_t * framebuffer = (uint8_t *) vbeModeInfo->framebuffer;
	uint64_t offset = (x * ((vbeModeInfo->bpp)/8)) + (y * vbeModeInfo->pitch);
	framebuffer[offset]     =  color.b;
	framebuffer[offset+1]   =  color.g;
	framebuffer[offset+2]   =  color.r;
	return OK;
}

int64_t drawRectangle(uint64_t x, uint64_t y, uint64_t width, uint64_t height, Color color){
    if(x+width > SCREEN_WIDTH || y+height > SCREEN_HEIGHT){
        return ERROR;
    }
    if(!inVideoMode()){
        return ERROR;
    }
    for (uint64_t i = 0; i < width; i++) {
        for (uint64_t j = 0; j < height; j++) {
            putPixel( x + i, y + j, color);
        }
    }
    return OK;
}

int64_t drawFont(uint64_t x, uint64_t y, uint8_t ch, Color color, uint64_t size) {
    if (ch < FIRST_ASCII_FONT || ch > LAST_ASCII_FONT) return -1;
    
    unsigned char * glyph = fontPixelMap(ch, fontBitmap);
    for (uint64_t row = 0; row < FONT_HEIGHT; row++) {
        unsigned char bits = glyph[row];
        for (uint64_t col = 0; col < FONT_WIDTH; col++) {
            if ((bits >> (7 - col)) & 1) {
                drawRectangle(x + col * size, y + row * size, size, size, color);
            }
        }
    }
    return OK;
}

void setFontColor(Color c){
    fontColor = c;
}

// Cambia el tamaño de fuente en MODO TEXTO
int64_t setFontSize(uint64_t size) {
    if(!inTextMode()){
        return ERROR;
    }

    if (size < 0 || SCREEN_WIDTH < size * FONT_WIDTH || SCREEN_HEIGHT < size * FONT_HEIGHT) {
        return ERROR;
    }
    if (size == fontSize) {
        return OK;
    }
    fontSize = size;
    charBufferRowsZoomed = (SCREEN_HEIGHT / (fontSize * FONT_HEIGHT));  // cantidad de caracteres maximos por linea
    charBufferColsZoomed = (SCREEN_WIDTH / (fontSize * FONT_WIDTH));	// cantidad de caracteres maximos por columna
    clearScreen();		// Limpia todo cuando se modifica el tamaño de fuente
    return OK;
}

int64_t colorClearScreen(Color color){
    if(driverMode == TEXT_MODE){
        backgroundColor = color;
        currentScreenPoint.x = currentScreenPoint.y = 0;
        bufferIndex = 0;
    }
    overrideMode = 1;
    drawRectangle(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, color);
    overrideMode = 0;
    return OK;
}

int64_t setMode(uint64_t mode, Color c){
    if(!(mode == TEXT_MODE || mode == VIDEO_MODE) ){
        return ERROR;
    }
    backgroundColor = c;
    if(mode == driverMode){
        return OK;
    }
    driverMode = mode;

    if(mode == TEXT_MODE){
        printBuffer();			// Printea el buffer cuando vuelve a modo texto
    }else{
    	colorClearScreen(c);
    }

    return OK;
}

int64_t textWrite(uint64_t fd, const char * buffer, int64_t amount){
    if(!inTextMode()){
        return OK;
    }
    if(!(fd == STDOUT || fd == STDERR)){
        return OK;
    }

    uint64_t i = 0;
    while (buffer[i] != 0 && i<amount) {
        switch (buffer[i]) {
            case '\n':
                newLine();
                break;
            case '\b':
                backSpace();
                break;
            case '\t':
                tabulator();
                break;
            default:
                if(buffer[i] >= FIRST_ASCII_FONT && buffer[i] <=LAST_ASCII_FONT){
                    charBufferType c = {buffer[i], fd};
                    addCharToBuffer(buffer[i], fd);
                    printFont(c);
                }
                break;
        }
        i++;
    }
    return i;
}

//Funciones STATIC:
static uint64_t inTextMode(){
    return ((driverMode == TEXT_MODE) || overrideMode);
}
static uint64_t inVideoMode(){
    return ((driverMode == VIDEO_MODE) || overrideMode);
}

static void tabulator(){
    charBufferType c = {' ', STDOUT};
    for(int i = 0; i < SPACE_PER_TAB ; i++){
        addCharToBuffer(' ', STDOUT);
        printFont(c);
    }
}

static void addCharToBuffer(uint8_t c, uint8_t fd) {
    if (bufferIndex  >= charBufferRowsZoomed*charBufferColsZoomed) {
        reBufferPrint();
    }
    charBufferType aux = {c, fd};
    charBuffer[bufferIndex] = aux;
    bufferIndex ++;
}

static void printFont(charBufferType letter){

    if (currentScreenPoint.x+FONT_WIDTH*fontSize - FONT_WIDTH >= SCREEN_WIDTH) {
        currentScreenPoint.y += FONT_HEIGHT*fontSize;
        currentScreenPoint.x = 0;
    }
    if(currentScreenPoint.y + FONT_HEIGHT*fontSize - FONT_HEIGHT >= SCREEN_HEIGHT){
        reBufferPrint();
    }
    overrideMode=1;
    Color col;
    if(letter.fd == STDOUT){
        col = fontColor;
    }else{
        col = stderrColor;
    }
    drawFont(currentScreenPoint.x, currentScreenPoint.y, letter.c, col, fontSize);
    overrideMode=0;
    currentScreenPoint.x+=FONT_WIDTH*fontSize;
}

static void clearScreen() {
    colorClearScreen(backgroundColor);
}

static void newLinePrint(){
    currentScreenPoint.x = 0;
    currentScreenPoint.y = currentScreenPoint.y+ FONT_HEIGHT * fontSize;
    if(currentScreenPoint.y + FONT_HEIGHT * fontSize - FONT_HEIGHT >= SCREEN_HEIGHT){
        reBufferPrint();
    }
}

static void newLineBuff(){
    int remainingOnRow = charBufferColsZoomed - (bufferIndex % charBufferColsZoomed);
    for(int i=0 ; i<remainingOnRow; i++){
        addCharToBuffer(' ', STDOUT);
    }
}

static void newLine(){
    newLineBuff();
    newLinePrint();
}

static void backSpacePrint(){
    if(currentScreenPoint.x == 0 && currentScreenPoint.y == 0){
        return;
    }
    if(currentScreenPoint.x == 0){

        currentScreenPoint.y -= fontSize * FONT_HEIGHT;  // y es dif a 0
        currentScreenPoint.x = SCREEN_WIDTH - fontSize * FONT_WIDTH;
    }else{
        currentScreenPoint.x -= fontSize * FONT_WIDTH;
    }
    overrideMode=1;
    drawRectangle(currentScreenPoint.x , currentScreenPoint.y, X_FONT_OFFSET, FONT_HEIGHT*fontSize, backgroundColor);
    overrideMode=0;
}

static void backSpaceBuffer(){
    if(bufferIndex != 0){
        charBuffer[bufferIndex].c = 0;
        charBuffer[--bufferIndex].fd = STDOUT;
    }
}

static void backSpace(){
    backSpacePrint();
    backSpaceBuffer();
}

static void reBufferPrint(){
    uint64_t aux = bufferIndex; // con el clear screen se setea en 0
    clearScreen();
    uint64_t j = 0;
    for(uint64_t i=charBufferColsZoomed * ROWS_TO_REBUFFER(charBufferRowsZoomed) ; i<aux;i++, j++){
        charBuffer[j] = charBuffer[i];
        printFont(charBuffer[j]);
    }
    bufferIndex = j;
}

static void printBuffer(){
    uint64_t aux = bufferIndex; // con clear screen se borra
    clearScreen();
    for(int i = 0 ; i < aux ; i++){
        textWrite(charBuffer[i].fd, &charBuffer[i].c, 1);
    }
}