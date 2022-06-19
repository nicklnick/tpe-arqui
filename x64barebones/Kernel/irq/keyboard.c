#include <keyboard.h>

extern char readKeyboard();		// en libasm.asm

/*-------- CONSTANTS --------*/
#define BUFFER_SIZE 200

/*--------- MACROS ----------*/
#define INCREASE_MOD(x,total)	(x) = ((x) + 1) % total;
#define DECREASE_MOD(x, total) 	x--;\
				if((x)<0)\
    					x=((total) + (x)) % (total);\
				else\
   					x = (x) % (total);

/*-------- STATIC FILE VARIABLES --------*/
static char keyBuffer[BUFFER_SIZE];             // Buffer de caracters de teclado
static int writePos;				// Posicion a escribir en el buffer
static int readPos;				// Posicion a consumir en el buffer
static int peekPos;			        // Posicion para observar en el buffer

/* Tabla de equivalencias entre makeCode y Ascii */
static char scanCodeTable[] = {
        UNMAPPED,UNMAPPED,'1','2','3','4','5','6','7','8',	
	'9','0','-','=','\b','\t','q','w','e','r',
	't','y','u','i','o','p','[',']','\n',UNMAPPED,
	'a','s','d','f','g','h','j','k','l',';',
	'\'','|',UNMAPPED,'|','z','x','c','v','b','n',
	'm',',','.','/',UNMAPPED,'*',UNMAPPED,' ',UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,'-',
	UNMAPPED,UNMAPPED,UNMAPPED,'+',UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED
};	


/*-------- CODE --------*/

/* Usa un array circular. Si se llega a la capacidad maxima, no sobre-escribe. */
char keyboard_handler(uint64_t * regDumpPos) 
{

	int c = readKeyboard();

	switch(c){
		case F5_SCAN_CODE:
			saveInfoReg(regDumpPos);	// caso: aprienta boton de captura de registros
			return NO_KEY;
		case ESCAPE_KEY:
			killAllProcesses();
			return NO_KEY;
		case F1_KEY:
			pauseCenterProcess();
			return NO_KEY;
		case F2_KEY:
			pauseLeftScreenProcess();
			return NO_KEY;
		case F3_KEY:
			pauseRightScreenProcess();
			return NO_KEY;
	}

	if(c<0 || c>=128)			// caso: codigo invalido 
		return NO_KEY;
	
	if(keyBuffer[writePos]!=0)		// caso: no hay espacio en el buffer
		return BUFFER_FULL;		
			
	c = scanCodeTable[c];			// convierto a ascii

	// ------ Caracteres especiales ------
	if(c=='\b'){
		DECREASE_MOD(writePos,BUFFER_SIZE)

		if(keyBuffer[writePos]!=0){
			keyBuffer[writePos] = 0; 			// elimino del buffer
			if(writePos + 1 == peekPos)			// me muevo para atras en el peek 
				DECREASE_MOD(peekPos, BUFFER_SIZE)
		}
		else
			INCREASE_MOD(writePos,BUFFER_SIZE)		// no habia nada en el buffer, vuelvo adonde estaba
		return DELETE_KEY;
	}

	// ------ Caracteres normales -------
	if(c != UNMAPPED){
		keyBuffer[writePos] = c;					// se agraga al buffer
		INCREASE_MOD(writePos,BUFFER_SIZE)	

		return VALID_KEY;
	}
	return UNMAPPED;
}


/* Consumo letra del buffer y sobreescribe con 0 para denotar una posicion vacia. */
char get_key()
{
	if(!checkIfAvailableKey())
		return 0;

	if(peekPos == readPos)          	// para que el peek no quede apuntando a nada
		INCREASE_MOD(peekPos,BUFFER_SIZE)	

	char c = keyBuffer[readPos];		// consumo letra 
	keyBuffer[readPos] = 0;

	INCREASE_MOD(readPos,BUFFER_SIZE)
	
	return c;
}


/* Como el get_key pero no lo consumo, es decir, no pone en 0 sino que lo deja. */
char peek_key()
{
	if(keyBuffer[peekPos]==0)		// corto sin aumentar
		return 0;

	char c = keyBuffer[peekPos];
	INCREASE_MOD(peekPos,BUFFER_SIZE)	
	return c;
}


char checkIfAvailableKey() {
	return keyBuffer[readPos] != 0;
}