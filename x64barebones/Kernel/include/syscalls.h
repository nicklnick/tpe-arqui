
#ifndef _SYSCALLS_H
#define _SYSCALLS_H


/*
 * << sys_write >>
 * ----------------------------------------------------------------------
 * Descripcion: Takes up to [count] bytes from [buf] 
 *              and writes to a file descriptor ([fd])
 * ----------------------------------------------------------------------
 * Recibe: 
 *      [fd] = file descriptor to write
 *      [buf] = string to be written
 *      [count] = number of letters to be written
 * Devuelve: 
 *      (uint) bytes written
 *      
 */
unsigned int sys_write(unsigned int fd, const char *buf, unsigned int count);


/*
 * << sys_write >>
 * ----------------------------------------------------------------------
 * Descripcion: Reads up to [count] bytes from [fd] and writes to [buf]
 * ----------------------------------------------------------------------
 * Recibe: 
 *      [fd] = file descriptor to read from
 *      [buf] = pointer to zone to leave what was read
 *      [count] = number of letters to be written
 * Devuelve: 
 *      (uint) bytes read
 */
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

/*
 * << sys_rtc >>
 * ----------------------------------------------------------------------
 * Description: Gets current hour(1) or current day(2)
 * ----------------------------------------------------------------------
 * Receives: 
 *      1 => get current hour
 *      2 => get current day
 * Returns: 
 *      current hour HHMMSS
 *      current day DDMMYY
 *      0 other option
 */
unsigned int sys_rtc(unsigned int option);

/*
 * << sys_write_to_screen >>
 * ----------------------------------------------------------------------
 * Description: Writes to screen
 * ----------------------------------------------------------------------
 * Receives: 
 *      [buf] = string to be written
 *      [count] = number of letters to be written
 * Returns: 
 *      (uint) bytes written
 */
unsigned int sys_write_to_screen(const char *buf, unsigned int count);

/*
 * << sys_read_from_screen >>
 * ----------------------------------------------------------------------
 * Description: Reads from screen
 * ----------------------------------------------------------------------
 * Receives: 
 *      [buf] = string to be read
 *      [count] = number of letters to be read
 * Returns: 
 *      (uint) bytes read
 */
unsigned int sys_read_from_screen(char *buf, unsigned int count);


#endif
