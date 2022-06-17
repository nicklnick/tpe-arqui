#include <video.h>
#include <syscalls.h>
 
void clearScreen(int start, int length, int step);

/* Decides which screen to clear */
unsigned int sys_clear_screen() 
{
	int screen = getCurrentScreen();
	switch(screen) {
		case STDOUT_LEFT:
			ACCESS_VAR(currentVideoOffset()) = ACCESS_VAR(currentVideoLeftOffset()) = START_LEFT;		// se resetan las pantallas
			clearScreen(START_LEFT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			break;

		case STDOUT_RIGHT:
			ACCESS_VAR(currentVideoOffset()) = START_LEFT;		// se resetan las pantallas
			ACCESS_VAR(currentVideoRightOffset()) = START_RIGHT;
			clearScreen(START_RIGHT, SPLIT_MODE_LENGTH, SPLIT_MODE_STEP);
			break;

		case STDOUT:
		default:
			ACCESS_VAR(currentVideoOffset()) = ACCESS_VAR(currentVideoLeftOffset()) = START_LEFT;		// se resetan las pantallas
			ACCESS_VAR(currentVideoRightOffset()) = START_RIGHT;
			clearScreen(START_LEFT, NORMAL_MODE_LENGTH, NORMAL_MODE_STEP);
	}

	return 0;
}

/* Clears screen */
void clearScreen(int start, int length, int step) 
{
	for(int i=start ; i < SCREEN_WIDTH * SCREEN_HEIGHT - start; ) {
		*(defaultVideo() + i++) = ' ';
		*(defaultVideo() + i++) = STDOUT_COLOR;

		if( i % length  == 0)											// salto a nueva linea, si llegue a fin
			i += step;
	}
}