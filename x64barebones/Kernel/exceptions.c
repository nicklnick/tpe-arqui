
#define ZERO_EXCEPTION_ID 0
#define ZERO_EXCEPTION_ERR_MSG "Se produjo una excepcion por division por cero!.\n"
#define ZERO_EXCEPTION_ERR_MSG_LEN 49

static void zero_division();

void exceptionDispatcher(int exception) {
	if (exception == ZERO_EXCEPTION_ID)
		zero_division();
}

static void zero_division() {
	int screen =  get_process_register_screen() + 1;		// se le suma 1 pues la (screen + 1) es la misma screen pero en rojo.
	sys_write(screen, ZERO_EXCEPTION_ERR_MSG, ZERO_EXCEPTION_ERR_MSG_LEN);
	
	// TODO: Volcado de registros, supongo que hay que ir al stack y fijarse.
	// TODO: Arrelgar el stack?

}