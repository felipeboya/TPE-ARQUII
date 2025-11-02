#include <library.h>
#include <shell.h>

static int var1 = 0;
static int var2 = 0;

int main() {
    uint64_t color = 0x000000;  // Negro (más apropiado para modo texto)
    //setMode(TEXT_MODE, color);     

    for (int i = 0; i < 10; i++){
        printPrompt();
    }

    // Test if BSS is properly set up
    if (var1 == 0 && var2 == 0)
        return 0xDEADC0DE;

    return 0xDEADBEEF;
}