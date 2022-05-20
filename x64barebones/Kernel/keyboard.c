#include <keyboard.h>
#include <stdint.h>
extern char readKeyboard();		// en libasm.asm

#define BUFFER_SIZE 100
#define TRUE 1
#define FALSE 0
#define UNMAPPED 1

// Buffer de caracters de teclado
static char keyBuffer[BUFFER_SIZE];
static int writePos;					// Posicion a escribir en el buffer
static int readPos;						// Posicion a consumir en el buffer
static int peekPos;						// Posicion para observar en el buffer

// Tabla de equivalencias entre makeCode y Ascii
static char scanCodeTable[]={
    UNMAPPED,UNMAPPED,'1','2','3','4','5','6','7','8',	
	'9','0','-','=','\b','\t','q','w','e','r',
	't','y','u','i','o','p','[',']','\n',UNMAPPED,
	'a','s','d','f','g','h','j','k','l',';',
	'\'','`',UNMAPPED,'\\','z','x','c','v','b','n',
	'm',',','.','/',UNMAPPED,'*',UNMAPPED,' ',UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,'-',
	UNMAPPED,UNMAPPED,UNMAPPED,'+',UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,UNMAPPED,
	UNMAPPED,UNMAPPED,UNMAPPED
};	

/*
	Devuelve: 1 si se escribio la entrada al buffer, 0 sino.
	Funcionamiento: usa un array circular. Si se llega a la 
	capacidad maxima, no sobre escribe. 
*/

char keyboard_handler() {

	int c = readKeyboard();

	if(c>=0 && c<128){					
		if(keyBuffer[writePos]!=0){									// estoy a punto de pisar la primera letra a leer					
			return FALSE;									 		// tengo que cortar, solo puedo escribir cuando la persona consuma mas del buffer		
		} 

		keyBuffer[writePos] = scanCodeTable[c];
		writePos = (writePos + 1) % BUFFER_SIZE;
		return TRUE;
	}
	return FALSE;
}


/*
	Devuelve: 0 si no consumio nada, el caracter si lo consumio
	Funcionamiento: consumo letra del buffer y sobreescribe con
	0 para denotar una posicion vacia.
*/

char get_key(){
	if(!checkIfAvailableKey())
		return 0;

	if(peekPos == readPos){							// para que el peek no quede apuntando a nada
		peekPos = (peekPos + 1) % BUFFER_SIZE;
	}

	char c = keyBuffer[readPos];
	keyBuffer[readPos] = 0;			
	readPos = (readPos + 1) % BUFFER_SIZE;


	
	return c;
}



/*
	Como el get_key pero no lo consumo, es decir, no pone en
	0 sino que lo deja. 
*/

char peek_key(){
	if(keyBuffer[peekPos]==0)
		return 0;

	char c = keyBuffer[peekPos];
	peekPos = (peekPos + 1) % BUFFER_SIZE;	
	return c;
}


char checkIfAvailableKey(){
	return keyBuffer[readPos]!=0;
}