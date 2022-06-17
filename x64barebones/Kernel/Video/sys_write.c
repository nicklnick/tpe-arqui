#include <video.h>

/* Decides where to write */
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
			write(buf, format, count, currentVideoOffset(), START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
			ACCESS_VAR(currentVideoRightOffset())=0;		// se resetean las split screen
			ACCESS_VAR(currentVideoLeftOffset())=0;
		break;

		case STDERR_LEFT:
		case STDOUT_LEFT:
			write(buf, format, count, currentVideoLeftOffset(), START_LEFT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			ACCESS_VAR(currentVideoOffset())=0;		// se resetean el normal mode
		break;

		case STDERR_RIGHT:
		case STDOUT_RIGHT:
			write(buf, format, count, currentVideoRightOffset(), START_RIGHT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			ACCESS_VAR(currentVideoOffset())=0;		// se resetean el normal mode
		break;
		
		default:			// el default ese la pantalla completa
			write(buf, format, count, currentVideoOffset(), START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
			ACCESS_VAR(currentVideoRightOffset())=0;		// se resetean las split screen
			ACCESS_VAR(currentVideoLeftOffset())=0;
	}
        return 0;       // lo agregue yo (nico) pq no habia nada, supongo que seria error
}


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
			*(defaultVideo() + (*offset)++) = c;			// escribo letra y formato
			*(defaultVideo() + (*offset)++) = format;

			if(*offset % length  == 0)						// salto a new line si llego a fin 
                *offset += step;
		}

	}
	return i;
}


/* ====== SPECIAL CHARACTERS FUNCTIONALITY ======*/
void deleteKey(unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step)
{
	if(*offset == start)			// si llegue al principio de la pantalla, no puedo ir para atras
		return;						

	if( ((*offset - 2) % SCREEN_WIDTH) < start || ((*offset - 2) % SCREEN_WIDTH) > start + length) // si estamos por fuera de los limites, entro devuelta pero una linea arriba
		*offset -= step;			
	
	*offset -= 2;					// voy uno para atras
	*(defaultVideo() + *offset) = ' ';	
}


void scrollUp(int start, int length, int step)
{
	
	for(int i=start, j = SCREEN_WIDTH + start; j < SCREEN_WIDTH * SCREEN_HEIGHT ;){				// Copio todo uno para arriba 1 fila
		*(defaultVideo() + i++) = *(defaultVideo() + j++); 

		if( i % length  == 0){											// salto a nueva linea, si llegue a fin
			i += step;
			j += step;
		}				
	}

	for(int i=(SCREEN_HEIGHT-1) * SCREEN_WIDTH + start; 				// elimino la ultima linea
		i < (SCREEN_HEIGHT-1) * SCREEN_WIDTH + length + start; i+=2)
		*(defaultVideo()+i)=' ';
}