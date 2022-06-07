#ifndef _COMANDOS_H
#define _COMANDOS_H
#include <stdint.h>

/*
 * << printmem >>
 * ----------------------------------------------------------------------
 * Descripcion: Prints 32bytes of memory starting from [position] 
 * ----------------------------------------------------------------------
 * Recibe:
 *      (uint64_t) starting position
 * Devuelve: -- 
 */
void printmem(char * arg);

/*
 * << primos >>
 * ----------------------------------------------------------------------
 * Descripcion: Prints prime values infinitely
 */
void primos();

/*
 * << fibonacci >>
 * ----------------------------------------------------------------------
 * Descripcion: Prints the fibonacci sequence
 */
void fibonacci();

/*
 * << help >>
 * ----------------------------------------------------------------------
 * Descripcion: Explains all commands and kb shortcuts
 */
void help();

/*
 * << time >>
 * ----------------------------------------------------------------------
 * Descripcion: Prints current RTC
 */
void time();

/*
 * << inforeg >>
 * ----------------------------------------------------------------------
 * Descripcion: Prints register values when a snapshot was taken
 */
void inforeg();

#endif

