#ifndef _SYSCALLS_H
#define _SYSCALLS_H

#include <stdint.h>

unsigned int sys_write(const char * string, int length);
unsigned int sys_read(char * buf, int length);
unsigned int sys_clear_screen();
unsigned int sys_register_process(uint64_t entryPoint, int screen);
unsigned int sys_rtc(uint64_t mode);

#endif