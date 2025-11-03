#define ZERO_EXCEPTION_ID 0

static void zeroDivision();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zeroDivision();
}

static void zeroDivision() {
	// Handler para manejar excepcíon
}