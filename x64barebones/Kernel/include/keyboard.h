#ifndef	_KEYBOARD_H
#define _KEYBOARD_H


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
