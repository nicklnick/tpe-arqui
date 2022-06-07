#include <syscalls.h>
#include <keyboard.h>
#include <multitasking.h>

// Entrada estandar
#define STDIN 1
#define STDIN_LEFT 3
#define STDIN_RIGHT 5

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

// Valores de retorno
#define INVALID_SCREEN -1

// Variables estaticas
static uint8_t * defaultVideoPos = (uint8_t*)0xB8000;

static unsigned int currentVideoPosOffset = START_LEFT;
static unsigned int currentVideoPosLeftOffset = START_LEFT;
static unsigned int currentVideoPosRightOffset = START_RIGHT;


/* ----------- CODIGO ------------ */

// ====== SYS_INFOREG =======

#define TOTAL_REGISTERS 15
static uint64_t inforegData[TOTAL_REGISTERS] = {0};

void saveInfoReg(uint64_t * regDumpPos){
	for(int i=0; i<TOTAL_REGISTERS; i++){
		inforegData[i] = regDumpPos[i];
	}
}

unsigned int sys_inforeg(uint64_t * buffer){			// recibo buffer y le escribo, NUNCA devuelvo puntero a algo que esta en kernel
	for(int i=0; i<TOTAL_REGISTERS; i++){
		buffer[i] = inforegData[i];
	}
	return 1;
}


// ====== SYS_PAUSE_PROCESS ======

unsigned int sys_pause_process(unsigned int pid){
	return pauseOrUnpauseProcess(pid);
}

// ====== SYS_KILL_PROCESS ======

unsigned int sys_kill_process(unsigned int pid){
	return removeTask(pid);
}

// ====== SYS_REGISTER_PROCESS ======

unsigned int sys_register_process(uint64_t entryPoint, int screen, uint64_t arg0){
	if(screen != STDOUT && screen != STDOUT_LEFT && screen != STDOUT_RIGHT)			// si no es una pantalla valida, se rechaza el proceso.
		return INVALID_SCREEN;
	return addTask(entryPoint, screen, arg0);
}

// ====== SYS_CLEAR_SCREEN ======

void clearScreen(int start, int length, int step){
	for(int i=start ; i < SCREEN_WIDTH * SCREEN_HEIGHT - start; ){
		*(defaultVideoPos + i++) = ' ';
		*(defaultVideoPos + i++) = STDOUT_COLOR;

		if( i % length  == 0){											// salto a nueva linea, si llegue a fin
			i += step;
		}			
	}
}

unsigned int sys_clear_screen(){
	int screen = getCurrentScreen();
	switch(screen){
		case STDOUT_LEFT:
			currentVideoPosOffset = currentVideoPosLeftOffset = START_LEFT;		// se resetan las pantallas
			clearScreen(START_LEFT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			break;
		case STDOUT_RIGHT:
			currentVideoPosOffset = START_LEFT;		// se resetan las pantallas
			currentVideoPosRightOffset = START_RIGHT;
			clearScreen(START_RIGHT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			break;
		case STDOUT:
		default:
			currentVideoPosOffset = currentVideoPosLeftOffset = START_LEFT;		// se resetan las pantallas
			currentVideoPosRightOffset = START_RIGHT;
			clearScreen(START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
	}

	return 0;
}
// ====== SYS_WRITE ======

/*
	Parametros:
	offset: posicion actual de "cursor "	
	start: columna inicial en pantalla (Ej: normal=izq=0, right= 80)		| length: longitud de pantalla en la que se imprime
	step: cantidad de posiciones a saltar en pantalla, al llegar al final de la pantalla

*/

void deleteKey(unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step){
	if(*offset == start)			// si llegue al principio de la pantalla, no puedo ir para atras
		return;

	if( ((*offset - 2) % SCREEN_WIDTH) < start || ((*offset - 2) % SCREEN_WIDTH) > start + length){	
		*offset -= step;			// si estamos por fuera de los limites, entro devuelta pero una linea arriba
	}		
	*offset -= 2;					// voy uno para atras
	*(defaultVideoPos + *offset) = ' ';	
}

/*
	Parametros:
	start: columna inicial en pantalla (Ej: normal=izq=0, right= 80)		| length: longitud de pantalla en la que se imprime
	step: cantidad de posiciones a saltar en pantalla, al llegar al final de la pantalla

*/

void scrollUp(int start, int length, int step)
{
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
	unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step)
{
	int i;

	for(i=0; i<count; i++) {

		if(*offset >= (SCREEN_HEIGHT-1) * SCREEN_WIDTH + length + start) {							// llego al final de pantalla, tengo que hacer scroll up
			scrollUp(start, length, step);
			*offset = (SCREEN_HEIGHT-1) * SCREEN_WIDTH + start;
		}
		
		char c = buf[i];

		//------ CARACTERES EPECIALES ------	
		if(c == '\n') {		
			int aux = length - (*offset % length);			// avanzo a la proxima linea en pantalla
			*offset += aux + step;
		}
		else if(c == '\b')				
			deleteKey(offset, start, length, step);		
		

		//------ CARACTERES NORMALES ------	
		else {	
			*(defaultVideoPos + (*offset)++) = c;			// escribo letra y formato
			*(defaultVideoPos + (*offset)++) = format;

			if(*offset % length  == 0)						// salto a new line si llego a fin 
                *offset += step;
		}

	}
	return i;
}


// ====== SYS_WRITE ======
unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count) 
{
	char format;

	if(fd % 2 != 0)				// notar que solo los pares son ERROR
		format=STDOUT_COLOR;
	else 
		format=STDERR_COLOR;

	switch(fd) {
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
		default:			// el default ese la pantalla completa
			write(buf, format, count, &currentVideoPosOffset, START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
			currentVideoPosRightOffset=0;		// se resetean las split screen
			currentVideoPosLeftOffset=0;
	}
        return 0;       // lo agregue yo (nico) pq no habia nada, supongo que seria error
}

// ====== SYS_CONSUME_STDIN ======

unsigned int sys_consume_stdin(char * buf, unsigned int count){
	int i=0;
	while(checkIfAvailableKey() && i<count){
		char c = get_key();
		buf[i++] = c;
	}
	return i;
}


// ====== SYS_READ ======

unsigned int getFdOffSet(unsigned int fd){
	switch(fd){
		case STDIN_LEFT:
			return currentVideoPosLeftOffset;
		case STDIN_RIGHT:
			return currentVideoPosRightOffset;
		case STDIN:
		default:
			return currentVideoPosOffset;
	}
}

unsigned int read_stdin(unsigned int fd, char * buf, unsigned int count) 
{
	char c=0, keyboardResp=0; 
	int i=0;
	unsigned int initialPos = getFdOffSet(fd);					// ### FEO ###
	while(c!='\n' && keyboardResp != BUFFER_FULL) {		

		keyboardResp = keyboard_handler();

		if(keyboardResp==VALID_KEY) {
			c = peek_key();
			sys_write(fd,&c, 1);
		
			if(i<count) 
				i++;
		}
		else if(keyboardResp == DELETE_KEY){
			if(getFdOffSet(fd) > initialPos){			// ### FEO ###   // no dejo que borre lo que ya habia
				sys_write(fd,"\b",1);
				if(i>0)
					i--;
			}
		}
	}	

	for(int j=0 ; j<i;j++){				// consumo el buffer de una, hasta el \n o fin de caracteres
		buf[j] = get_key();
	}

	return i;
}


// Solo copia
unsigned int sys_read(unsigned int fd, char * buf, unsigned int count)
{
	switch(fd) {										// Eligimos posicion de donde leer. Tambien lo podriamos hacer con una funcion/tabla
		case STDIN:
		case STDIN_LEFT:
		case STDIN_RIGHT:
			if(checkIfAvailableKey()){
				return sys_consume_stdin(buf,count);		// Si el key buffer no esta vacio, primero tengo que consumirlo
			}
			return read_stdin(fd, buf, count);				// El buffer esta vacio, puedo leer de pantalla
		default:
			return 0;	// Seria error?
	}
}


// ====== RTC ====== 
/* constants */
#define HOUR 1
#define DATE 2
#define SUCCESS 1

/* fields */
#define F_SEC   0x00
#define F_MIN   0x02
#define F_HOURS 0x04

#define F_DAY   0x07
#define F_MONTH 0x08
#define F_YEAR  0x09

extern int getRTC(uint8_t field);

unsigned int sys_rtc(unsigned int option) {
        int hours, min, sec;
        int day, month, year;

        switch(option) {
                // Me piden horario
                // Devuelve HHMMSS
                case HOUR:
                        hours = getRTC(F_HOURS);     // hours
                        hours = ( (hours & 0x0F) + (((hours & 0x70) / 16) * 10) ) | (hours & 0x80);

                        min = getRTC(F_MIN);         // min
                        min = (min & 0x0F) + ((min / 16) * 10);

                        sec = getRTC(F_SEC);         // sec
                        sec = (sec & 0x0F) + ((sec / 16) * 10);

                        return (hours - 3) * 10000 + min * 100 + sec;

                // Me piden fecha
                // Devuelve DDMMYY
                case DATE:
                        day = getRTC(F_DAY);             // day
                        day = (day & 0x0F) + ((day / 16) * 10);

                        month = getRTC(F_MONTH);       // month
                        month = (month & 0x0F) + ((month / 16) * 10);

                        year = getRTC(F_YEAR);        // year
                        year = (year & 0x0F) + ((year / 16) * 10);
                        return day * 10000 + month * 100 + year;
        }
        return 0;       // error?
}

// ====== SYS_WRITE_TO_SCREEN ======

unsigned int sys_write_to_screen(const char *buf, unsigned int count){
	return sys_write(getCurrentScreen(),buf,count);			// no requiere de la especificacion de a que pantalla escribir 
}


// ====== SYS_READ_FROM_SCREEN ======

unsigned int sys_read_from_screen(char *buf, unsigned int count){
	return sys_read(getCurrentScreen(),buf,count);			// no requiere de la especificacion de a que pantalla escribir 
}

