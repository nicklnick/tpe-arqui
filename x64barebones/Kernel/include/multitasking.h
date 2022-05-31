#ifndef _MULTITASKING_H
#define _MULTITASKING_H

void enableMultiTasking();

int addTask(uint64_t entrypoint, int screen);
void moveToNextTask(uint64_t stackPointer, uint64_t stackSegment);
void removeCurrentTask();
int removeTask(unsigned int pid);

uint64_t getRSP();
uint64_t getSS();
uint8_t getCurrentScreen();


#endif
