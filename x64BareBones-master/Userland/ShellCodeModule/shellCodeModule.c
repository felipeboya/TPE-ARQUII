#include <library.h>
#include <shell.h>

static int var1 = 0;
static int var2 = 0;

int main() {
    // Inicia en modo texto con fondo negro 

    write(STDOUT, "Welcome! Run the command 'help' to see all the commands\n");

    char buffer[BUFF_SIZE];

    while (1) {
        printPrompt();
        readLine(buffer);
        processLine(buffer);
    }

    // Test if BSS is properly set up
    if (var1 == 0 && var2 == 0)
        return 0xDEADC0DE;

    return 0xDEADBEEF;
}