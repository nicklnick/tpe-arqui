#include <stdint.h>

void * memset(void * destination, int32_t c, uint64_t length)
{
	uint8_t chr = (uint8_t)c;
	char * dst = (char*)destination;

	while(length--)
		dst[length] = chr;

	return destination;
}

void * memcpy(void * destination, const void * source, uint64_t length)
{
	/*
	* memcpy does not support overlapping buffers, so always do it
	* forwards. (Don't change this without adjusting memmove.)
	*
	* For speedy copying, optimize the common case where both pointers
	* and the length are word-aligned, and copy word-at-a-time instead
	* of byte-at-a-time. Otherwise, copy by bytes.
	*
	* The alignment logic below should be portable. We rely on
	* the compiler to be reasonably intelligent about optimizing
	* the divides and modulos out. Fortunately, it is.
	*/
	uint64_t i;

	if ((uint64_t)destination % sizeof(uint32_t) == 0 &&
		(uint64_t)source % sizeof(uint32_t) == 0 &&
		length % sizeof(uint32_t) == 0)
	{
		uint32_t *d = (uint32_t *) destination;
		const uint32_t *s = (const uint32_t *)source;

		for (i = 0; i < length / sizeof(uint32_t); i++)
			d[i] = s[i];
	}
	else
	{
		uint8_t * d = (uint8_t*)destination;
		const uint8_t * s = (const uint8_t*)source;

		for (i = 0; i < length; i++)
			d[i] = s[i];
	}

	return destination;
}

int str_len(const char * string){
	int i = 0;
    while(string[i] != '\0'){
        i++;
    }
    return i;
}

void reverseString(char * buffer){
	int i, j;
    char aux;
    j = str_len(buffer)-1;
    for(i = 0 ; i<j ; i++, j--){
        aux = buffer[i];
        buffer[i] = buffer[j];
        buffer[j] = aux;
    }

}

int hex_to_string(uint64_t num, char * buffer, int fixedLength){
	int i = 0;
    for(int aux ; num > 0 ; i++, num/=16){
       	aux = num % 16;
       	if(aux >=0 && aux < 10)						// convierto a hex
       		buffer[i] = aux + '0';
       	else
       		buffer[i] = aux - 10 + 'A';

    }
    while(i<fixedLength){					// le agrego 0 por deltante para llegar a la longitud deseada
    	buffer[i++] = '0';
    }
    buffer[i] = 0;
    reverseString(buffer);
    return i;
}

