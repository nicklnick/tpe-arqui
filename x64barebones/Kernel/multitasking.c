#include <stdint.h>
#include <multitasking.h>
#include <interrupts.h>

#define RET_POS 8

#define SS_POS	  (2*8)				// usa padding
#define SP_POS 	  (3*8)	
#define FLAGS_POS (4*8)	
#define CS_POS 	  (5*8)
#define IP_POS    (6*8)

#define STACK_POINT_OF_ENTRY (21*8)   // posicion de donde se puede restaurar los registros y hacer iretq 



static unsigned int currentTask;
static int dimTasks = -1;
static taskInfo tasks[TOTAL_TASKS];

// -----El stack que usa cada task-----
static uint8_t stack1[STACK_SIZE];
static uint8_t stack2[STACK_SIZE];
static uint8_t stack3[STACK_SIZE];
static uint8_t stack4[STACK_SIZE];

static uint8_t * stacks[TOTAL_TASKS] = {stack1, stack2, stack3, stack4};

// -----El defualt stack/task-------
// Este es la info del stack que se inicializa 
// al comienzo de la ejecucion del kernel
static taskInfo defaultStack;

static uint8_t currentPid;

static uint8_t isEnabled;



/* --------- CODIGO ---------- */

void enableMultiTasking(){
	isEnabled = 1;
	forceCurrentTask();
}

uint8_t multitaskingEnabled(){
	return (dimTasks != -1) && isEnabled;
}


void moveToNextTask(uint64_t stackPointer, uint64_t stackSegment){
	if(!multitaskingEnabled()){										// si no hay tasks, se usa el normal
		defaultStack.stackPointer = stackPointer;			// Pongo los datos del stack normal, en la primera posicion
		defaultStack.stackSegment = stackSegment;
		defaultStack.screen = 1;
		defaultStack.pid = currentPid++;
		return;
	}

	tasks[currentTask].stackPointer = stackPointer;			// updateo el current
	tasks[currentTask].stackSegment = stackSegment;
	
	char found=0;
	for(unsigned int i=currentTask; !found ; ){			// busco el proximo stack
		i = (i +  1) % TOTAL_TASKS;
		if(tasks[i].isActive){
			currentTask = i;
			found = 1;
		}	
	}
	
}

uint64_t getRSP(){
	if(!multitaskingEnabled()){					// si no hay tasks, se usa el normal
		return defaultStack.stackPointer;
	}
	return tasks[currentTask].stackPointer;
}
uint64_t getSS(){
	if(!multitaskingEnabled()){					// si no hay tasks, se usa el normal
		return defaultStack.stackSegment;
	}
	return tasks[currentTask].stackSegment;
}
uint8_t getCurrentScreen(){
	return tasks[currentTask].screen;
}

void removeCurrentTask(){
	tasks[currentTask].isActive = 0;
	dimTasks = dimTasks==1 ? -1 : dimTasks - 1;
	forceNextTask();				
}



int addTask(uint64_t entrypoint, int screen){
	if(dimTasks>=TOTAL_TASKS){		// no acepto mas tasks al estar lleno
		return -1;
	}
	dimTasks = dimTasks == -1 ? 1 : dimTasks+1;

	int pos;
	for(pos=0; tasks[pos].isActive ; pos++);													// busco espacio vacio en array de tasks

	*((uint64_t*) (stacks[pos] + STACK_SIZE - RET_POS)) = (uint64_t) &removeCurrentTask;				// para el RET que vaya y se remueva automaticamente
	
	*((uint64_t*) (stacks[pos] + STACK_SIZE - SS_POS)) = 0x0000000000000000;
	*((uint64_t*) (stacks[pos] + STACK_SIZE - SP_POS)) = stacks[pos] + STACK_SIZE - RET_POS;		// agarro el comienzo del stack
	
	*((uint64_t*) (stacks[pos] + STACK_SIZE - FLAGS_POS)) = 0x202;				// tenemos que poner el flag de interrupcion en 1 y otros obligatorios

	*((uint64_t*) (stacks[pos] + STACK_SIZE - CS_POS)) = 0x0000000000000008;				
	*((uint64_t*) (stacks[pos] + STACK_SIZE - IP_POS)) = entrypoint;

	tasks[pos].stackPointer = stacks[pos] + STACK_SIZE - STACK_POINT_OF_ENTRY;
	tasks[pos].stackSegment = 0;		

	tasks[pos].screen = screen;
	tasks[pos].pid = currentPid++;
	tasks[pos].isActive = 1;
	return tasks[pos].pid;
}


