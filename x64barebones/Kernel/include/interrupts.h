 /*
 *   interrupts.h
 *
 *  Created on: Apr 18, 2010
 *      Author: anizzomc
 */

#ifndef INTERRUPS_H_
#define INTERRUPS_H_

#include <idtLoader.h>
#include <stdint.h>         // ? REMOVE ?

/* IRQ HANDLERS */
void _irq00Handler(void);
void _irq01Handler(void);
void _irq02Handler(void);
void _irq03Handler(void);
void _irq04Handler(void);
void _irq05Handler(void);

/* EXCEPTION HANDLERS */
void _exception0Handler(void);
void _exception6Handler(void);

/* ASM */
void _cli(void);
void _sti(void);
void _hlt(void);

/* PIC */
void picMasterMask(uint8_t mask);
void picSlaveMask(uint8_t mask);

/* Termina la ejecución de la cpu */
void haltcpu(void);


void forceNextTask();
void _swIntHandler(void);       // ES VOID????


#endif /* INTERRUPS_H_ */
