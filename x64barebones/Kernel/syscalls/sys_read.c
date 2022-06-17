#include <syscalls.h>
#include <video.h>

unsigned int getFdOffSet(unsigned int fd);
unsigned int read_stdin(unsigned int fd, char * buf, unsigned int count);

// solo copia
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


unsigned int getFdOffSet(unsigned int fd)
{
	switch(fd){
		case STDIN_LEFT:
			return ACCESS_VAR(currentVideoLeftOffset());

		case STDIN_RIGHT:
			return ACCESS_VAR(currentVideoRightOffset());

		case STDIN:
		default:
			return ACCESS_VAR(currentVideoOffset());
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
			sys_write(fd,&c, 1);
		
			if(i<count) 
				i++;
		}
		else if(keyboardResp == DELETE_KEY) {
			if(getFdOffSet(fd) > initialPos){  // no dejo que borre lo que ya habia
				sys_write(fd,"\b",1);
				if(i>0)
					i--;
			}
		}
	}	

	for(int j=0 ; j<i;j++)				// consumo el buffer de una, hasta el \n o fin de caracteres
		buf[j] = get_key();

	return i;
}