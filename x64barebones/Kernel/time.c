#include <time.h>

static unsigned long ticks = 0;

void timer_handler() {
	ticks++;
	sys_write(3,"t",1);
}

int ticks_elapsed() {
	return ticks;
}

int seconds_elapsed() {
	return ticks / 18;
}
