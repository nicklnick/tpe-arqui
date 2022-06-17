#include <video.h>

void deleteKey(unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step);
void scrollUp(int start, int length, int step);

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


/*
	Parametros:
	offset: posicion actual de "cursor "	
	start: columna inicial en pantalla (Ej: normal=izq=0, right= 80)		| length: longitud de pantalla en la que se imprime
	step: cantidad de posiciones a saltar en pantalla, al llegar al final de la pantalla

*/
void deleteKey(unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step)
{
	if(*offset == start)			// si llegue al principio de la pantalla, no puedo ir para atras
		return;						

	if( ((*offset - 2) % SCREEN_WIDTH) < start || ((*offset - 2) % SCREEN_WIDTH) > start + length) // si estamos por fuera de los limites, entro devuelta pero una linea arriba
		*offset -= step;			
	
	*offset -= 2;					// voy uno para atras
	*(defaultVideo() + *offset) = ' ';	
}

/*
	Parametros:
	start: columna inicial en pantalla (Ej: normal=izq=0, right= 80)		| length: longitud de pantalla en la que se imprime
	step: cantidad de posiciones a saltar en pantalla, al llegar al final de la pantalla

*/
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