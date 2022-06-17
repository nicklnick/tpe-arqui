#include <video.h>

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
