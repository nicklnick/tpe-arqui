#include <stdint.h>
#include <multitasking.h>

#define STACK_POINT_OF_ENTRY 3    // posicion de donde se puede restaurar los registros y hacer iretq 
#define IP_POS 4

static unsigned int currentTask;
static unsigned int dimTasks;
static taskInfo tasks[TOTAL_TASKS];

static stack stacks[TOTAL_TASKS];

// -----El defualt stack/task-------
// Este es la info del stack que se inicializa 
// al comienzo de la ejecucion del kernel
static taskInfo defaultStack;

static uint8_t currentPid;				//


/* --------- CODIGO ---------- */

void initializeMultiTasking(uint8_t * stackPointer, uint8_t * stackSegment){	
	defaultStack.stackPointer = stackPointer;						// Pongo los datos del stack normal, en la primera posicion
	defaultStack.stackSegment = stackSegment;
	defaultStack.screen = 1;
	defaultStack.pid = currentPid++;
}

taskInfo getNextTask(){
	if(!dimTasks){					// si no hay tasks, se usa el normal
		return defaultStack;
	}
	taskInfo resp = tasks[currentTask];

	currentTask = (currentTask + 1) % dimTasks;

	return resp;
}


void addTask(uint64_t entrypoint, int screen){
	if(dimTasks>=TOTAL_TASKS){		// no acepto mas tasks al estar lleno
		return;
	}

	dimTasks++;

	//TODO: meter el entrypoint en la posicion correcta;

	*(staks[dimTasks].stackStart + STACK_SIZE - IP_POS) = entrypoint;		// NO ME VA A DEJAR HACER ESTO (CREO)

	// agarro el comienzo del stack (que seria el final pq va disminuyendo RSP)
	tasks[dimTasks].stackPointer = stacks[dimTasks].stackStart + STACK_SIZE - STACK_POINT_OF_ENTRY;		

	tasks[dimTasks].stackSegment = 0;					//????????????????
	tasks[dimTasks].screen = screen;
	tasks[dimTasks].pid = currentPid++;


}
void removeTask(){		// ?????????????

}
