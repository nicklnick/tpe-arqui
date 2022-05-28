#include <stdint.h>
#include <multitasking.h>

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


/* --------- CODIGO ---------- */

void initializeMultiTasking(uint64_t stackPointer, uint64_t stackSegment){	
	defaultStack.stackPointer = stackPointer;						// Pongo los datos del stack normal, en la primera posicion
	defaultStack.stackSegment = stackSegment;
	defaultStack.screen = 1;
	defaultStack.pid = currentPid++;
}

void moveToNextTask(){
	if(dimTasks == -1){					// si no hay tasks, se usa el normal
		return;
	}
	
	char found=0;
	for(unsigned int i=currentTask; !found ; ){
		i = ( i + 1 ) % TOTAL_TASKS;
		if(tasks[i].isActive){
			currentTask = i;
			found = 1;
		}
		found = i + '0';

		sys_write(3,&found,1);
	}

	taskInfo resp = tasks[currentTask];
}

uint64_t getRSP(){
	if(dimTasks == -1){					// si no hay tasks, se usa el normal
		return defaultStack.stackPointer;
	}
	return tasks[currentTask].stackPointer;
}
uint64_t getSS(){
	if(dimTasks == -1){					// si no hay tasks, se usa el normal
		return defaultStack.stackSegment;
	}
	return tasks[currentTask].stackSegment;
}
uint64_t getDimTask(){
	return dimTasks;
}

void removeTask(){
	tasks[currentTask].isActive = 0;
}

int addTask(uint64_t entrypoint, int screen){
	if(dimTasks>=TOTAL_TASKS){		// no acepto mas tasks al estar lleno
		return -1;
	}

	dimTasks = dimTasks == -1 ? 1 : dimTasks+1;

	int pos;
	for(pos=0; tasks[pos].isActive ; pos++);													// busco espacio vacio en array de tasks

	*((uint64_t*) (stacks[pos] + STACK_SIZE - RET_POS)) = (uint64_t) &removeTask;				// para el RET que vaya y se remueva automaticamente
	
	*((uint64_t*) (stacks[pos] + STACK_SIZE - SS_POS)) = 0x0000000000000000;
	*((uint64_t*) (stacks[pos] + STACK_SIZE - SP_POS)) = stacks[pos] + STACK_SIZE - SS_POS;		// agarro el comienzo del stack (que seria el final pq va disminuyendo RSP)
	
	*((uint64_t*) (stacks[pos] + STACK_SIZE - FLAGS_POS)) = 0x0000000000000000;

	*((uint64_t*) (stacks[pos] + STACK_SIZE - CS_POS)) = 0x0000000000000008;				
	*((uint64_t*) (stacks[pos] + STACK_SIZE - IP_POS)) = entrypoint;

	tasks[pos].stackPointer = stacks[pos] + STACK_SIZE - STACK_POINT_OF_ENTRY;
	tasks[pos].stackSegment = 0;		

	tasks[pos].screen = screen;
	tasks[pos].pid = currentPid++;
	tasks[pos].isActive = 1;

	return tasks[pos].pid;
}


