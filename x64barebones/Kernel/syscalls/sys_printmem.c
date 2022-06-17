#include <syscalls.h>

// ====== SYS_PRINTMEM ========
#define MAX_MEM_READ 16
#define BYTE_LENGTH 2
#define MAX_MEM_POS 250000000

unsigned int sys_printmem(uint64_t position, char * buffer){
	if(position >= MAX_MEM_POS)
		return -1;
	uint64_t current;
	
	for(int i=0, k=0; i < MAX_MEM_READ; i++) {
		if(i!=0 && i%4==0)
			buffer[k++] = ' ';

       		current = *((uint8_t * )position + i);
        	k += hex_to_string(current, buffer + k, BYTE_LENGTH);
	}

	return 0;
}