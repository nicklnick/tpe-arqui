#include <syscalls.h>
#include <stdint.h>

#define STDOUT 1
#define STDERR 2

#define STDOUT_LEFT 3
#define STDOUT_RIGHT 5
#define STDERR_LEFT 4
#define STDERR_RIGHT 6

#define STDOUT_COLOR 7
#define STDERR_COLOR 4


static uint8_t * const defaultVideoPos = (uint8_t*)0xB8000;

static unsigned int currentVideoPosOffset = 0;
static unsigned int currentVideoPosLeftOffset = 0;
static unsigned int currentVideoPosRightOffset = 80;


// 160 x 25 
unsigned int write_left(const char * buf, char format, unsigned int count){
	int i;

	for(i=0; i<count; i++){
		int aux = currentVideoPosLeftOffset;
		if( aux % 160  == 80)
			currentVideoPosLeftOffset += 80;

		*(defaultVideoPos + currentVideoPosLeftOffset++) = buf[i];
		*(defaultVideoPos+ currentVideoPosLeftOffset++) = format;

		for(int k=0; k<50000000; k++);
	}
	return i;
}

unsigned int write_right(const char * buf, char format, unsigned int count){
	int i;

	for(i=0; i<count; i++){
		int aux = currentVideoPosRightOffset;
		if( aux % 160  == 0)
			currentVideoPosRightOffset += 80;

		*(defaultVideoPos + currentVideoPosRightOffset++) = buf[i];
		*(defaultVideoPos+ currentVideoPosRightOffset++) = format;

		for(int k=0; k<50000000; k++);
	}
	return i;
}


unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count){
	char format;

	if(fd % 2 != 0)				// notar que solo los pares son ERROR
		format=STDOUT_COLOR;
	else 
		format=STDERR_COLOR;

	write_left(buf,format,count);
	write_right(buf,format,count);	
	return 0;
}
