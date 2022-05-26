
#include <stdint.h>
#include <libasm.h>
#include <lib.h>

#define ZERO_EXCEPTION_ID 0
#define ZERO_EXCEPTION_ERR_MSG "Se produjo una excepcion por division por cero!.\n"
#define ZERO_EXCEPTION_ERR_MSG_LEN 49

#define INVALID_OP_CODE 6
#define INVALID_OP_CODE_ERR_MSG "Se produjo una excepcion por codigo de operacion invalida.\n"
#define INVALID_OP_CODE_MSG_LEN 59

#define TOTAL_REGISTERS 15
#define REGISTER_LENGTH 16

static char * registerOrder[] = {
	"RAX: ","RBX: ","RCX: ","RDX: ",
	"RBP: ","RSP: ","RSI: ","RDI: ",
	"R8: ", "R9: ","R10: ","R11: ",
	"R12: ","R13: ","R14: ","R15: " 
};

void printRegisters(int screen, uint64_t * registerDumpPos){

	char buffer[100];
	for(int i=0, j=TOTAL_REGISTERS-1; i<TOTAL_REGISTERS ; i++, j--){

		sys_write(screen, registerOrder[i], str_len(registerOrder[i]));			// imprimo que registro es

		int amount = hex_to_string(registerDumpPos[j], buffer, REGISTER_LENGTH);					
		sys_write(screen, buffer,amount);										// imprimo valor de registro
		sys_write(screen, "\n",1);
	}
}

void zero_division(uint64_t * registerDumpPos) {
	int screen =  get_process_register_screen() + 1;		// se le suma 1 pues la (screen + 1) es la misma screen pero en rojo.
	sys_write(screen, ZERO_EXCEPTION_ERR_MSG, ZERO_EXCEPTION_ERR_MSG_LEN);
	
	printRegisters(screen,registerDumpPos);

	// TODO: Arrelgar el stack?

}

void invalid_op_code(uint64_t * registerDumpPos){
	int screen =  get_process_register_screen() + 1;
	sys_write(screen, INVALID_OP_CODE, INVALID_OP_CODE_MSG_LEN);

	printRegisters(screen,registerDumpPos);

}

void exceptionDispatcher(int exception, uint64_t * registerDumpPos) {
	switch(exception){
		case ZERO_EXCEPTION_ID:
			zero_division(registerDumpPos);
			break;
		case INVALID_OP_CODE:
			invalid_op_code(registerDumpPos);
			break;
	}
}
