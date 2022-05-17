#include <keyboard.h>
#include <stdint.h>
extern char readKeyboard();		// en libasm.asm

#define BUFFER_SIZE 100
#define FLOOR_MOD(x, total) 	if((x)<0)							\
				    				x=((total) + (x)) % (total); 	\
								else 								\
				   					 x = (x) % (total);				\

// Buffer de caracters de teclado
static char keyBuffer[BUFFER_SIZE];
static int posInBuffer=0;

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
		if(posInBuffer!=0)
			posInBuffer = (posInBuffer + 1) % BUFFER_SIZE;		// hacemos un circular array, asi si pisa lo mas viejo si se pasa del size

		keyBuffer[posInBuffer] = scanCodeTable[c];		
	}

}

char get_key(){
	if(checkIfAvailableKey()==0)
		return 0;

	char c = keyBuffer[posInBuffer];
	keyBuffer[posInBuffer] = 0;							// agarro letra y la elimino, para delimitar los posiciones vacias

	posInBuffer--;										// me muevo uno para atras.
	FLOOR_MOD(posInBuffer, BUFFER_SIZE)					// % no coopera con numeros negativos

	return c;
}

char checkIfAvailableKey(){
	return keyBuffer[posInBuffer]!=0;
}