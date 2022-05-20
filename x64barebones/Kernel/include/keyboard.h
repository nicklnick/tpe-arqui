#ifndef	_KEYBOARD_H
#define _KEYBOARD_H

char keyboard_handler();
char get_key();                 /* Obtiene la key y la consume */
char checkIfAvailableKey();     
char peek_key();                /* Obtiene la key pero NO la consume */

#endif
