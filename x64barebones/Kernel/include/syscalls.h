
#ifndef _SYSCALLS_H
#define _SYSCALLS_H

/* Escribe hasta count bytes a un fd (file descriptor) lo que le pasan por buf */
unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count);
/* Lee hasta count bytes de un fd (file descriptor) y deja lo leido en buf */
unsigned int sys_read(unsigned int fd, char * buf, unsigned int count);

#endif
