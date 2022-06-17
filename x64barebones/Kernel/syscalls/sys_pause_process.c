#include <syscalls.h>

// ====== SYS_PAUSE_PROCESS ======
unsigned int sys_pause_process(unsigned int pid) {
	return pauseOrUnpauseProcess(pid);
}
