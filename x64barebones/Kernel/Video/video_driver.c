#include <keyboard.h>
#include <multitasking.h>
#include <video.h>

// Variables estaticas
static uint8_t * defaultVideoPos = (uint8_t*)0xB8000;

static unsigned int currentVideoPosOffset = START_LEFT;
static unsigned int currentVideoPosLeftOffset = START_LEFT;
static unsigned int currentVideoPosRightOffset = START_RIGHT;


/* Writes to given screen */
unsigned int write(const char * buf, char format, unsigned int count, 
	unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step)
{
	int i;

	for(i=0; i<count; i++) {

		if(*offset >= (SCREEN_HEIGHT-1) * SCREEN_WIDTH + length + start) {	// llego al final de pantalla, tengo que hacer scroll up
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


/* Decides how to proceed depending on screen to write */
unsigned int writeDispatcher(unsigned int fd, const char * buf, unsigned int count) 
{
	char format;

	if(fd % 2 != 0)				// notar que solo los pares son ERROR
		format = STDOUT_COLOR;
	else 
		format = STDERR_COLOR;

	switch(fd) {
		case STDERR:							// mismo codigo
		case STDOUT:
			write(buf, format, count, &currentVideoPosOffset, START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
			currentVideoPosRightOffset = 0;		// se resetean las split screen
			currentVideoPosLeftOffset = 0;
		break;

		case STDERR_LEFT:
		case STDOUT_LEFT:
			write(buf, format, count, &currentVideoPosLeftOffset, START_LEFT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			currentVideoPosOffset = 0;		// se resetean el normal mode
		break;

		case STDERR_RIGHT:
		case STDOUT_RIGHT:
			write(buf, format, count, &currentVideoPosRightOffset, START_RIGHT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			currentVideoPosOffset = 0;		// se resetean el normal mode
		break;
		
		default:			// el default ese la pantalla completa
			write(buf, format, count, &currentVideoPosOffset, START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
			currentVideoPosRightOffset = 0;		// se resetean las split screen
			currentVideoPosLeftOffset = 0;
	}

    return 0;
}


/* Clears given screen */
void clearScreen(int start, int length, int step) 
{
	for(int i=start ; i < SCREEN_WIDTH * SCREEN_HEIGHT - start; ) {
		*(defaultVideoPos + i++) = ' ';
		*(defaultVideoPos + i++) = STDOUT_COLOR;

		if( i % length  == 0)											// salto a nueva linea, si llegue a fin
			i += step;
	}
}

/* Decides how to proceed depending on screen to clear */
unsigned int clearScreenDispatcher() 
{
	int screen = getCurrentScreen();
	switch(screen) {
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


/* Gets the offset of a given fd */
unsigned int getFdOffSet(unsigned int fd)
{
	switch(fd) {
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
	unsigned int initialPos = getFdOffSet(fd);

	while(c!='\n' && keyboardResp != BUFFER_FULL) {		
		keyboardResp = keyboard_handler();

		if(keyboardResp==VALID_KEY) {
			c = peek_key();
			writeDispatcher(fd,&c, 1);
		
			if(i<count) 
				i++;
		}
		else if(keyboardResp == DELETE_KEY) {
			if(getFdOffSet(fd) > initialPos) {  // no dejo que borre lo que ya habia
				writeDispatcher(fd,"\b",1);
				if(i>0)
					i--;
			}
		}
	}	

	for(int j=0 ; j<i;j++)				// consumo el buffer de una, hasta el \n o fin de caracteres
		buf[j] = get_key();

	return i;
}


/* Decides how to proceed depending on where to read */
unsigned int readDispatcher(unsigned int fd, char * buf, unsigned int count) 
{
	switch(fd) {										// Eligimos posicion de donde leer. Tambien lo podriamos hacer con una funcion/tabla
		case STDIN:
		case STDIN_LEFT:
		case STDIN_RIGHT:
			if(checkIfAvailableKey())
				return consume_stdin(buf,count);		// Si el key buffer no esta vacio, primero tengo que consumirlo
			return read_stdin(fd, buf, count);				// El buffer esta vacio, puedo leer de pantalla

		default:
			return 0;	// Seria error?
	}
}


void deleteKey(unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step)
{
	if(*offset == start)			// si llegue al principio de la pantalla, no puedo ir para atras
		return;						

	if( ((*offset - 2) % SCREEN_WIDTH) < start || ((*offset - 2) % SCREEN_WIDTH) > start + length) // si estamos por fuera de los limites, entro devuelta pero una linea arriba
		*offset -= step;			
	
	*offset -= 2;					// voy uno para atras
	*(defaultVideoPos + *offset) = ' ';	
}


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

unsigned int consume_stdin(char * buf, unsigned int count) {
	int i=0;
	
	while(checkIfAvailableKey() && i<count) {
		char c = get_key();
		buf[i++] = c;
	}
	return i;
}