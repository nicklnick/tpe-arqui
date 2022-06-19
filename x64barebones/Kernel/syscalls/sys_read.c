#include <keyboard.h>
#include <syscalls.h>
#include <video.h>


// solo copia
unsigned int sys_read(unsigned int fd, char * buf, unsigned int count)
{
	switch(fd) {										// Eligimos posicion de donde leer. Tambien lo podriamos hacer con una funcion/tabla
		case STDIN:
		case STDIN_LEFT:
		case STDIN_RIGHT:
			if(checkIfAvailableKey())
				return sys_consume_stdin(buf,count);		// Si el key buffer no esta vacio, primero tengo que consumirlo
			return read_stdin(fd, buf, count);				// El buffer esta vacio, puedo leer de pantalla

		default:
			return 0;	// Seria error?
	}
}
