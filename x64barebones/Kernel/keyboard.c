#include <keyboard.h>
#include <stdint.h>
extern char readKeyboard();		// en libasm.asm

#define BUFFER_SIZE 100
#define FLOOR_MOD(x, total) 	if((x)<0)							\
				    				x=((total) + (x)) % (total); 	\
								else 								\
				   					 x = (x) % (total);	

// Buffer de caracters de teclado
static char keyBuffer[BUFFER_SIZE] = {0};
static int writePos=0;
static int readPos=0;

// Tabla de equivalencias entre makeCode y Ascii
static char scanCodeTable[]={
    0,0,'1','2','3','4','5','6','7','8',	
	'9','0','-','=','\b','\t','q','w','e','r',
	't','y','u','i','o','p','[',']','\n',0,
	'a','s','d','f','g','h','j','k','l',';',
	'\'','`',0,'\\','z','x','c','v','b','n',
	'm',',','.','/',0,'*',0,' ',0,0,0,0,0,
	0,0,0,0,0,0,0,0,0,0,0,'-',0,0,0,'+',0,0,
	0,0,0,0,0,0,0,0,0
};	

void keyboard_handler() {

	char c = readKeyboard();

	if(c>=0 && c<128){
		if(writePos==readPos){
			int prevToRead = readPos-1;														// posicion previa a readPos
			FLOOR_MOD(prevToRead, BUFFER_SIZE)

			if(keyBuffer[prevToRead]!=0)													// estoy a punto de pisar la primera letra a leer
				readPos = (readPos + 1) % BUFFER_SIZE;	 									// tengo que avanzar read asi no se corta el texto
		} 

		keyBuffer[writePos] = scanCodeTable[c];
		writePos = (writePos + 1) % BUFFER_SIZE;
	}
}

char get_key(){
	if(!checkIfAvailableKey())
		return 0;

	char c = keyBuffer[readPos];
	keyBuffer[readPos] = 0;			
	readPos = (readPos + 1) % BUFFER_SIZE;					

	return c;
}

char checkIfAvailableKey(){
	return keyBuffer[readPos]!=0;
}