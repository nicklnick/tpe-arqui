#ifndef	_KEYBOARD_H
#define _KEYBOARD_H

/*--------  DEPENDENCIES --------*/
#include <stdint.h>
#include <syscalls.h>
#include <multitasking.h>

/* --- CONSTANTS  --- */
#define NO_KEY 0
#define VALID_KEY 1
#define DELETE_KEY 2
#define BUFFER_FULL 3
#define UNMAPPED 4

// --- Caracteres especiales ---
#define ESCAPE_KEY 0x2
#define F1_KEY 0x3B
#define F2_KEY 0x3C
#define F3_KEY 0x3D
#define F5_SCAN_CODE 0x3F


/*
 * << keyboard_handler >>
 * ----------------------------------------------------------------------
 *  Descripcion: Handles keyboard functionality
 * ----------------------------------------------------------------------
 *  Recibe: --
 *  Devuelve:
 *      1 <=> wrote buffer
 *      0 <=> otherwise
 */
char keyboard_handler();


/*
 * << get_key >>
 * ----------------------------------------------------------------------
 * Descripcion: OBTAINS && CONSUMES pressed key
 * ----------------------------------------------------------------------
 * Recibe: --
 * Devuelve: 
 *      (char) pressedKey <=> was a mapped key and consumed it
 *      0 <=> otherwise
 */
char get_key();


/*
 * << peek_key >>
 * ----------------------------------------------------------------------
 * Descripcion: OBTAINS but DOES NOT CONSUME pressed key
 * ----------------------------------------------------------------------
 * Recibe: --
 * Devuelve:
 *      (char) pressedKey <=> was a mapped key
 *      0 <=> otherwise
 */
char peek_key();

char checkIfAvailableKey();

#endif
