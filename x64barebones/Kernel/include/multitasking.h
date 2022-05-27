#ifndef _MULTITASKING_H
#define _MULTITASKING_H

#define TOTAL_TASKS 4
#define STACK_SIZE 1000

// -----Informacion sobre cada task-----
typedef struct taskInfo{
		uint8_t *  stackPointer;		// puntero a byte, es decir, una posicion de memoria
		uint8_t *  stackSegment;
		uint8_t screen;
		uint8_t pid;
}taskInfo;

// -----El stack que usa cada task-----
typedef struct stack{
		uint8_t stackStart[STACK_SIZE];
}stack;

taskInfo getNextTask();
void addTask(uint64_t entrypoint, int screen);

#endif