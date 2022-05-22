
#ifndef _SYSCALLS_H
#define _SYSCALLS_H

unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count);
unsigned int sys_read(unsigned int fd, char * buf, unsigned int count);

/*
 * << sys_clear_screen >>
 * ----------------------------------------------------------------------
 * Description: Clears STODUT
 * ----------------------------------------------------------------------
 * Receives: --
 * Returns: 
 *      0 if successful 
 */
unsigned int sys_clear_screen();

#endif
