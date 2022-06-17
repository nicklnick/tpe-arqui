#include <syscalls.h>

// ====== SYS_KILL_PROCESS ======
unsigned int sys_kill_process(unsigned int pid){
	return removeTask(pid);
}