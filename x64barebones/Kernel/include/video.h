#ifndef _VIDEO_H
#define _VIDEO_H

#include <syscalls.h>

// Columna de comienzo en pantalla
#define START_LEFT 0
#define START_RIGHT 80

// Dimensiones de pantalla
#define NORMAL_MODE_LENGTH 160
#define SPLIT_MODE_LENGTH 80

#define SCREEN_HEIGHT 25
#define SCREEN_WIDTH 160

// Salto de pantalla
#define NORMAL_MODE_STEP 0
#define SPLIT_MODE_STEP 80

// Colores 
#define STDOUT_COLOR 7
#define STDERR_COLOR 4

#define ACCESS_VAR(var) (*(var))

// setup.c
// variables usadas en el driver de video
uint8_t * defaultVideo();
unsigned int * currentVideoOffset();
unsigned int * currentVideoLeftOffset();
unsigned int * currentVideoRightOffset();

/*
 * << write >>
 * ----------------------------------------------------------------------
 * Descripcion: Writes to corresponding screen
 * ----------------------------------------------------------------------
 * Recibe: 
 *      [buf] = string to be written
 * 		[format] = background color
 *      [count] = number of letters to be written
 *		[offset] = current cursor position
 * 		[start] = initial column of screen
 *		[length] = length of screen where we print
 *		[step] = number of positions to jump after we reach end of sceen
 * Devuelve: 
 *      (uint) bytes written
 */
unsigned int write(const char * buf, char format, unsigned int count, 
					unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step);

/*
 * << deleteKey >>
 * ----------------------------------------------------------------------
 * Descripcion: DeleteKey functionality
 * ----------------------------------------------------------------------
 * Recibe: 
 *		[offset] = current cursor position
 * 		[start] = initial column of screen (Eg: normal=izq=0, right= 80)
 *		[length] = length of screen where we print
 *		[step] = number of positions to jump after we reach end of sceen
 * Devuelve: --
 */
void deleteKey(unsigned int * offset, unsigned int start,  unsigned int length , unsigned int step);

/*
 * << scrollUp >>
 * ----------------------------------------------------------------------
 * Descripcion: ScrollUp functionality
 * ----------------------------------------------------------------------
 * Recibe: 
 * 		[start] = initial column of screen (Eg: normal=izq=0, right= 80)
 *		[length] = length of screen where we print
 *		[step] = number of positions to jump after we reach end of sceen
 * Devuelve: --
 */
void scrollUp(int start, int length, int step);
#endif