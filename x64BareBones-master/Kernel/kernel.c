#include <stdint.h>
#include <string.h>
#include <lib.h>
#include <moduleLoader.h>
#include <naiveConsole.h>
#include <idtLoader.h>

#include <keyboard.h>
#include <video.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize)
{
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	char buffer[10];

	ncPrint("[x64BareBones]");
	ncNewline();

	ncPrint("CPU Vendor:");
	ncPrint(cpuVendor(buffer));
	ncNewline();

	ncPrint("[Loading modules]");
	ncNewline();
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);
	ncPrint("[Done]");
	ncNewline();
	ncNewline();

	ncPrint("[Initializing kernel's binary]");
	ncNewline();

	clearBSS(&bss, &endOfKernel - &bss);

	ncPrint("  text: 0x");
	ncPrintHex((uint64_t)&text);
	ncNewline();
	ncPrint("  rodata: 0x");
	ncPrintHex((uint64_t)&rodata);
	ncNewline();
	ncPrint("  data: 0x");
	ncPrintHex((uint64_t)&data);
	ncNewline();
	ncPrint("  bss: 0x");
	ncPrintHex((uint64_t)&bss);
	ncNewline();

	ncPrint("[Done]");
	ncNewline();
	ncNewline();
	return getStackBase();
}

// int main()
// {	
// 	ncPrint("[Kernel Main]");
// 	ncNewline();
// 	ncPrint("  Sample code module at 0x");
// 	ncPrintHex((uint64_t)sampleCodeModuleAddress);
// 	ncNewline();
// 	ncPrint("  Calling the sample code module returned: ");
// 	ncPrintHex(((EntryPoint)sampleCodeModuleAddress)());
// 	ncNewline();
// 	ncNewline();

// 	ncPrint("  Sample data module at 0x");
// 	ncPrintHex((uint64_t)sampleDataModuleAddress);
// 	ncNewline();
// 	ncPrint("  Sample data module contents: ");
// 	ncPrint((char*)sampleDataModuleAddress);
// 	ncNewline();

// 	ncPrint("[Finished]");
// 	return 0;
// }

// IMPLEMENTACIÓN DE PRUEBA (BORRAR)
uint64_t strlen(const char *str);

int main(){
    
	// Por default arranca en modo texto, con backgroundColor negro, fontColor blanco, fontSize 1
    
    // Imprime el mensaje en pantalla (MODO TEXTO)
    const char *bienvenida = "Sistema Operativo Iniciado\n";
    textWrite(STDOUT, bienvenida, strlen(bienvenida));
    
    // Cargar tabla de interrupciones
    load_idt();

	// Cambiar a modo video
	Color negro = {0, 0, 0};
	setMode(VIDEO_MODE, negro);

	// Dibujar pixel
	Color rojo = {255, 0, 0};
	putPixel(100, 100, rojo);

	// Dibujar rectángulo
	Color verde = {0, 255, 0};
	drawRectangle(50, 50, 200, 100, verde);

	// Dibujar texto en modo gráfico
	Color blanco = {255, 255, 255};
	drawFont(10, 10, 'A', blanco, 2); 

	// Llama al módulo (todavia no anda porque hay que cambiar los makefile)
    // int resultado = ((EntryPoint)sampleCodeModuleAddress)();

	// Vuelve a modo texto (limpia la pantalla) y espera interrupciones
	setMode(TEXT_MODE, negro);
    while(1){
        _hlt();  // Esperar interrupciones
    }
    
    return 0;
}


uint64_t strlen(const char *str) {
    uint64_t len = 0;
    while (str[len] != '\0') {
        len++;
    }
    return len;
}