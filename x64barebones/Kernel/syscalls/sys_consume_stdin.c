#include <syscalls.h>
#include <video.h>


unsigned int sys_consume_stdin(char * buf, unsigned int count) {
	return consume_stdin(buf, count);
}
