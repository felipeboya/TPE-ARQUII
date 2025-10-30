#include <library.h>

static int var1 = 0;
static int var2 = 0;

int main() {
	
	char buffer[32] = {0};

    write(1, "Escribe algo: ");		
    // read(0, buffer, 31);     // leer hasta 31 caracteres

	int i = 0, t = 0;
	while(t < 100000000000){
		read(0, buffer, 31);
		write(1, buffer);
		t++;
	};

    write(1, "\nLeiste: ");
    write(1, buffer);

	//Test if BSS is properly set up
	if (var1 == 0 && var2 == 0)
		return 0xDEADC0DE;

	return 0xDEADBEEF;
}