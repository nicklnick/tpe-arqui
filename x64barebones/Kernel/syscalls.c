#include <syscalls.h>
#include <stdint.h>
#include <keyboard.h>

// Normal mode
#define STDOUT 1
#define STDERR 2

// Split screen
#define STDOUT_LEFT 3
#define STDOUT_RIGHT 5
#define STDERR_LEFT 4
#define STDERR_RIGHT 6

// Colores 
#define STDOUT_COLOR 7
#define STDERR_COLOR 4

// Offsets
#define OFFSET 0
#define OFFSET_RIGHT 80


static uint8_t * defaultVideoPos = (uint8_t*)0xB8000;

//static unsigned int currentVideoPosOffset = OFFSET;
///static unsigned int currentVideoPosLeftOffset = OFFSET;
//static unsigned int currentVideoPosRightOffset = OFFSET_RIGHT;


// =========================VERSION 1==========================


unsigned int write_to_side(const char * buf, char format, unsigned int count, unsigned int * offset , unsigned int length , int step){
	int i;

	for(i=0; i<count; i++){
		
		char c = buf[i];

		//--CARACTERES EPECIALES--	
		if(c=='\n'){							// avanzo a la proxima linea
			int aux = length - (*offset % length);
			*offset += aux + step;
		}
		else{
			*(defaultVideoPos + (*offset)++) = c;
			*(defaultVideoPos + (*offset)++) = format;
		}
		if( *offset % length  == 0)				// si from es 80 => left mode, from es 0 => right / normal mode
			*offset += step;					// si step es 0 => normal mode, si es 80 => right/left mode
	}
	return i;
}


void scrollUp(int from, int to){				
	for(int i=0, j = 160 ; j < 160 * 25 ;i++, j++){				// Copio todo uno para 
		*(defaultVideoPos + i) = *(defaultVideoPos + j); 
	}
}


// ====== SYSWRITE ======

unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count){
	char format;

	if(fd % 2 != 0)				// notar que solo los pares son ERROR
		format=STDOUT_COLOR;
	else 
		format=STDERR_COLOR;

	int offset = 0;
	*defaultVideoPos = 'K';

	offset = 16;
	write_to_side(buf, format, count, &offset, 160,0);

	for(int i=0; i<500000000; i++);

	scrollUp(0,0);

	return 0;
}






// ====== SYSREAD ======


// Por ahora solo hacemos para STDIN
unsigned int sys_read(unsigned int fd, char * buf, unsigned int count){
	// int i;

	// for(i=0; i<count && c!='\n'; ){
	// 	if(checkIfAvailableKey()){
	// 		char c = get_key();
	// 		buf[i] = c;
	// 		i++;
	// 		sys_write(STDOUT,&c, 1);
	// 	}
	// }

	// return i;
	return 0;
}
