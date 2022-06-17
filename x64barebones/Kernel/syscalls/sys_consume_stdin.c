#include <syscalls.h>

// ====== SYS_CONSUME_STDIN ======
unsigned int sys_consume_stdin(char * buf, unsigned int count)
{
	int i=0;
	
	while(checkIfAvailableKey() && i<count) {
		char c = get_key();
		buf[i++] = c;
	}
	return i;
}
