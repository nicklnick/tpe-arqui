#ifndef _MULTITASKING_H
#define _MULTITASKING_H

#define TOTAL_TASKS 4
#define STACK_SIZE 2000

// -----Informacion sobre cada task-----
typedef struct taskInfo{
		uint64_t  stackPointer;		// puntero a byte, es decir, una posicion de memoria
		uint64_t  stackSegment;  
		uint8_t screen;
		uint8_t pid;
		uint8_t isActive;			// si el proceso es uno activo o ya se elimino
}taskInfo;

void moveToNextTask(uint64_t stackPointer, uint64_t stackSegment);
int addTask(uint64_t entrypoint, int screen);
void enableMultiTasking();

uint64_t getRSP();
uint64_t getSS();

void removeCurrentTask();

#endif