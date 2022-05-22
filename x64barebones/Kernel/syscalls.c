#include <syscalls.h>
#include <stdint.h>
#include <keyboard.h>

// Entrada estandar
#define STDIN 0

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

// Columna de comienzo en pantalla
#define START_LEFT 0
#define START_RIGHT 80

// Dimensiones de pantalla
#define NORMAL_MODE_LENGTH 160
#define SPLIT_MODE_LENGTH 80

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 160

// Salto de pantalla
#define NORMAL_MODE_STEP 0
#define SPLIT_MODE_STEP 80



static uint8_t * defaultVideoPos = (uint8_t*)0xB8000;

static unsigned int currentVideoPosOffset = START_LEFT;
static unsigned int currentVideoPosLeftOffset = START_LEFT;
static unsigned int currentVideoPosRightOffset = START_RIGHT;


// =========================VERSION 1==========================

/*
	Parametros:
	start: columna inicial en pantalla (Ej: normal=izq=0, right= 80)		| length: longitud de pantalla en la que se imprime
	step: cantidad de posiciones a saltar en pantalla, al llegar al final de la pantalla

*/

void scrollUp(int start, int length, int step){			

	for(int i=start, j = SCREEN_WIDTH + start; j < SCREEN_WIDTH * SCREEN_HEIGHT ;){				// Copio todo uno para arriba 1 fila
		*(defaultVideoPos + i++) = *(defaultVideoPos + j++); 

		if( i % length  == 0){											// salto a nueva linea, si llegue a fin
			i += step;
			j += step;
		}				
	}

	for(int i=(SCREEN_HEIGHT-1) * SCREEN_WIDTH + start; 				// elimino la ultima linea
		i < (SCREEN_HEIGHT-1) * SCREEN_WIDTH + length + start; i+=2)
		*(defaultVideoPos+i)=' ';
}

/* 
	Parametros:
	buf: texto que se debe escribir en pantalla 							| format: color/fondo de texto
	offset: posicion actual de "cursor "									| count: cantidad de letras en texto
	start: columna inicial en pantalla (Ej: normal=izq=0, right= 80)		| length: longitud de pantalla en la que se imprime
	step: cantidad de posiciones a saltar en pantalla, al llegar al final de la pantalla

*/
unsigned int write(const char * buf, char format, unsigned int count, 
	unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step){

	int i;

	for(i=0; i<count; i++){

		if(*offset >= (SCREEN_HEIGHT-1) * SCREEN_WIDTH + length + start){							// llego al final de pantalla, tengo que hacer scroll up
			scrollUp(start, length, step);
			*offset = (SCREEN_HEIGHT-1) * SCREEN_WIDTH + start;
		}
		
		char c = buf[i];

		//--CARACTERES EPECIALES--	
		if(c=='\n'){										// CASO: hay un \n en el texto
			int aux = length - (*offset % length);			// avanzo a la proxima linea en pantalla
			*offset += aux + step;
		}

		else{	
			*(defaultVideoPos + (*offset)++) = c;			// escribo letra y formato
			*(defaultVideoPos + (*offset)++) = format;

			if( *offset % length  == 0)						// salto a new line si llego a fin 
			*offset += step;
		}
	}
	return i;
}

void clearScreen(){
	for(int i=0 ; i < SCREEN_WIDTH * SCREEN_HEIGHT ; i+=2){				// Copio todo uno para arriba 1 fila
		*(defaultVideoPos + i) = ' ';			
	}
}

// ====== SYSWRITE ======

unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count){
	char format;

	if(fd % 2 != 0)				// notar que solo los pares son ERROR
		format=STDOUT_COLOR;
	else 
		format=STDERR_COLOR;


	// ###### REMOVE #######
	if(currentVideoPosOffset==0 && currentVideoPosRightOffset==80 && currentVideoPosLeftOffset==0)
		clearScreen();

	switch(fd){
		case STDERR:							// mismo codigo
		case STDOUT:
			write(buf, format, count, &currentVideoPosOffset, START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
			currentVideoPosRightOffset=0;		// se resetean las split screen
			currentVideoPosLeftOffset=0;
		break;

		case STDERR_LEFT:
		case STDOUT_LEFT:
			write(buf, format, count, &currentVideoPosLeftOffset, START_LEFT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			currentVideoPosOffset=0;		// se resetean el normal mode
		break;

		case STDERR_RIGHT:
		case STDOUT_RIGHT:
			write(buf, format, count, &currentVideoPosRightOffset, START_RIGHT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			currentVideoPosOffset=0;		// se resetean el normal mode
		break;

	}
}

// ====== SYSREAD ======

unsigned int read_stdin(char * buf, unsigned int count){
	char c=0; 
	int i=0;
	while(c!='\n'){
		if(keyboard_handler()){
			c = peek_key();
			sys_write(1,&c, 1);
		
			if(i<count-1){
				buf[i] = get_key();
				i++;
			}
		}
	}	
	buf[i]=0;
	
	return i;
}

unsigned int consume_stdin(char * buf, unsigned int count){
	int i=0;
	while(checkIfAvailableKey() && i<count-1){
		buf[i++] = get_key();
	}
	buf[i]=0;
	return i;
}

// Solo copia
unsigned int sys_read(unsigned int fd, char * buf, unsigned int count){

	switch(fd){								// Eligimos posicion de donde leer. Tambien lo podriamos hacer con una funcion/tabla
		case STDIN:
			if(checkIfAvailableKey()){
				return consume_stdin(buf,count);		// Si el key buffer no esta vacio, primero tengo que consumirlo
			}
			return read_stdin(buf, count);				// El buffer esta vacio, puedo leer de pantalla
		default:
			return 0;	// Seria error?
	}
}
