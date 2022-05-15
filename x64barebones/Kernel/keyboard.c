#include <keyboard.h>

extern char readKeyboard();		// en libasm.asm

// ----Para despues----
static char keyBuffer[100];
static int posInKeys=0;

// El ascii key actual 
static char key;

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
		key = scanCodeTable[c];
	}
}

char get_key(){
	return key;
}