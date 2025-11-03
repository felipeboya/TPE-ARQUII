#include <stdint.h>
#define ZERO_EXCEPTION_ID 0
//#define SIX_EXCEPTION_ID 6

static void zeroDivision();

void exceptionDispatcher(uint64_t exception) {
    if (exception == ZERO_EXCEPTION_ID)
		zeroDivision();
// 	printRegs();
// 	if (exception == ZERO_EXCEPTION_ID){

// 	}else if(exception == SIX_EXCEPTION_ID){

}

// 	vamos a imprimir registros
// 	esperar a tecla
// }

static void zeroDivision() {
	// Handler para manejar excepcíon
}