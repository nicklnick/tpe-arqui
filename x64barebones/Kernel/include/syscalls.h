#ifndef _SYSCALLS_H
#define _SYSCALLS_H

/*--------  DEPENDENCIES --------*/
#include <stdint.h>


/* --- CONSTANTS  --- */
#define SYS_READ_FROM_SCREEN 0
#define SYS_WRITE_TO_SCREEN 1
#define SYS_CLEAR_SCREEN 2
#define SYS_REGISTER_PROCESS 3 
#define SYS_RTC 4
#define SYS_CONSUME_STDIN 7
#define SYS_KILL_PROCESS 8
#define SYS_PAUSE_PROCESS 9
#define SYS_INFOREG 10
#define SYS_PRINTMEM 11

// Entrada estandar
#define STDIN 1
#define STDIN_LEFT 3
#define STDIN_RIGHT 5

// Normal mode
#define STDOUT 1
#define STDERR 2

// Split screen
#define STDOUT_LEFT 3
#define STDOUT_RIGHT 5
#define STDERR_LEFT 4
#define STDERR_RIGHT 6

// Return values
#define INVALID_SCREEN -1


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
 * << sys_read >>
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


/*
 * << sys_register_process >>
 * ----------------------------------------------------------------------
 * Description: Registers a processs to participate in multitasking
 * ----------------------------------------------------------------------
 * Receives: 
 *      (uint64_t) start of function
 *      (int) screen to print
 *      (uint64_t) argument of function (if any)
 * Returns: 
 *      (uint) pid
 */
unsigned int sys_register_process(uint64_t entryPoint, int screen, uint64_t arg0);

/*
 * << sys_kill_process >>
 * ----------------------------------------------------------------------
 * Description: Eliminates a task from multitasking
 * ----------------------------------------------------------------------
 * Receives: 
 *      (uint64_t) pid of process to kill
 * Returns: 
 *      (uint) 1 if it was killed
 *             -1 if no task was found
 */
unsigned int sys_kill_process(unsigned int pid);

/*
 * << sys_pause_process >>
 * ----------------------------------------------------------------------
 * Description: Pauses or unpauses a task
 * ----------------------------------------------------------------------
 * Receives: 
 *      (uint64_t) pid of process to pause/unpause
 * Returns: 
 *      (uint) 1 if it was killed
 *             -1 if no task was found
 */
unsigned int sys_pause_process(unsigned int pid);


/*
 * << sys_consume_stdin >>
 * ----------------------------------------------------------------------
 * Description: Consumes buffer without interrupting to receive keyboard
 * ----------------------------------------------------------------------
 * Receives: 
 *      (char*) buffer to leave keys consumed
 *      (uint) amount of keys to consume
 * Returns: 
 *      (uint) amount of keys consumed
 */
unsigned int sys_consume_stdin(char * buf, unsigned int count);    

/*
 * << saveInfoReg >>
 * ----------------------------------------------------------------------
 * Description: Makes a snapshot of register values at the time of calling
 * this function
 * ----------------------------------------------------------------------
 * Receives: 
 *      (uint64_t*) place where all registers can be found
 * Returns: --
 */
void saveInfoReg(uint64_t * regDumpPos);

/*
 * << sys_inforeg >>
 * ----------------------------------------------------------------------
 * Description: Transfers saved register data from saveInfoReg into a buffer
 * ----------------------------------------------------------------------
 * Receives: 
 *      (char*) buffer to leave register values
 * Returns: --
 */
unsigned int sys_inforeg(uint64_t * buffer);

/*
 * << sys_printmem >>
 * ----------------------------------------------------------------------
 * Description: Memory dump starting from [position]
 * ----------------------------------------------------------------------
 * Receives: 
 *      (uint64_t) starting position of mem dump
 *      (char*) buffer to leave data
 * Returns: --
 */
unsigned int sys_printmem(uint64_t position, char * buffer);


#endif
