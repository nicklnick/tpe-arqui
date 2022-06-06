#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>

unsigned int sys_write(const char * string, int length);
unsigned int sys_read(char * buf, int length);
unsigned int sys_clear_screen();
unsigned int sys_register_process(uint64_t entryPoint, int screen, uint64_t arg0);
unsigned int sys_rtc(unsigned int mode);
unsigned int sys_consume_stdin(char * buf, unsigned int count);
unsigned int sys_kill_process(unsigned int pid);
unsigned int sys_pause_process(unsigned int pid);
unsigned int sys_inforeg(uint64_t * buffer);

#endif