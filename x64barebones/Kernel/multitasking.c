#include <stdint.h>
#include <multitasking.h>
#include <interrupts.h>

// ---- Constantes ----
#define TOTAL_TASKS 4
#define STACK_SIZE 2000

#define ACTIVE_PROCESS 1 
#define INACTIVE_PROCESS 0

#define NO_TASKS -1
#define ERROR_NO_SPACE_FOR_TASK -1

// ---- Valores default para el armado del stack ----
#define FLAG_VALUES 0x202
#define SS_VALUE 0
#define CS_VALUE 8

// ------Posiciones para el armado de stack para cada proceso------
										/*		 -=-=STACK=-=-		*/
#define STACK_POINT_OF_ENTRY (21*8)   	/*  	|	RAX, RBX  |		*/
										/*  	|	RCX, etc  |		*/   
										/*		---------------		*/ 
#define IP_POS    (6*8)					/*  	|	 RIP	  |		*/				
#define CS_POS 	  (5*8)					/*  	|	  CS	  |		*/
#define FLAGS_POS (4*8)					/*  	|	 RFLAGS	  |		*/
#define SP_POS 	  (3*8)					/*  	|	 RSP	  |		*/
#define SS_POS	  (2*8)					/*  	|	  SS	  |		*/
#define RET_POS 	8 					/*  	|	 RET	  |		*/
										/*   	 -------------		*/

// -----El stack que usa cada task-----
static uint8_t stack1[STACK_SIZE];
static uint8_t stack2[STACK_SIZE];
static uint8_t stack3[STACK_SIZE];
static uint8_t stack4[STACK_SIZE];

static uint8_t * stacks[TOTAL_TASKS] = {stack1, stack2, stack3, stack4};


// -----Informacion sobre cada task-----
typedef struct taskInfo{
		uint64_t  stackPointer;		// valor de rsp 
		uint64_t  stackSegment;  	// valor de ss
		uint8_t screen;				// en que pantalla va a imprimir
		uint8_t pid;				// valor unico identificador
		uint8_t isActive;			// si el proceso es uno activo o ya se elimino
}taskInfo;

// ------ Queue de tasks -------
static taskInfo tasks[TOTAL_TASKS];

static uint8_t currentPid;				// identificador para cada proceso
static uint8_t isEnabled;				// denota si multitasking se habilito

static unsigned int currentTask;		

static int dimTasks = NO_TASKS;

// -----El defualt stack/task-------
// Este es la info del stack que se inicializa 
// al comienzo de la ejecucion del kernel
static taskInfo defaultStack;



/* =========== CODIGO =========== */

/*
	Se habilita el multitasking e instantaneamente
	pasa al primer task en el queue.
*/
void enableMultiTasking(){
	isEnabled = 1;
	forceCurrentTask();
}

/*
	Se fija si se habilito el multitasking.
*/
uint8_t multitaskingEnabled(){
	return (dimTasks != NO_TASKS) && isEnabled;
}


/*	
	Pasa al proximo task que se tiene que ejecutar. 
	Parametros:  stackPointer: puntero al stack del task anterior  |  stackSegment: valor del stack segment del task anterior  
*/
void moveToNextTask(uint64_t stackPointer, uint64_t stackSegment){
	if(!multitaskingEnabled()){										// si no hay tasks, se usa el normal
		defaultStack.stackPointer = stackPointer;					// Pongo los datos del stack normal, en la primera posicion
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


/*
	Consigue el Stack Pointer del proceso 
	actual, es decir, el que se esta a punto
	de ejecutar.
*/
uint64_t getRSP(){
	if(!multitaskingEnabled()){					// si no hay tasks, se usa el normal
		return defaultStack.stackPointer;
	}
	return tasks[currentTask].stackPointer;
}



/*
	Consigue el Stack Segment del proceso 
	actual, es decir, el que se esta a punto
	de ejecutar.
*/
uint64_t getSS(){
	if(!multitaskingEnabled()){					// si no hay tasks, se usa el normal
		return defaultStack.stackSegment;
	}
	return tasks[currentTask].stackSegment;
}


/*	
	Consigue la pantalla a la cual el proceso
	actual escribe.
*/
uint8_t getCurrentScreen(){
	return tasks[currentTask].screen;
}


/*	
	Elimina el current task y pasa al proximo. 
*/
void removeCurrentTask(){
	tasks[currentTask].isActive = INACTIVE_PROCESS;
	dimTasks = dimTasks==1 ? NO_TASKS : dimTasks - 1;
	forceNextTask();				
}


/*	
	Agrega una funcion al queue de tasks. 
	Parametros:  entrypoint: puntero a funcion  |  screen: en que pantalla va a imprimir
*/

int addTask(uint64_t entrypoint, int screen){
	if(dimTasks>=TOTAL_TASKS){		// no acepto mas tasks al estar lleno
		return ERROR_NO_SPACE_FOR_TASK;
	}
	dimTasks = dimTasks == NO_TASKS ? 1 : dimTasks+1;

	int pos;
	for(pos=0; tasks[pos].isActive ; pos++);													// busco espacio vacio en array de tasks


	*((uint64_t*) (stacks[pos] + STACK_SIZE - RET_POS)) = (uint64_t) &removeCurrentTask;		// para el RET que vaya y se remueva automaticamente de los tasks
	
	*((uint64_t*) (stacks[pos] + STACK_SIZE - SS_POS)) = SS_VALUE;
	*((uint64_t*) (stacks[pos] + STACK_SIZE - SP_POS)) = stacks[pos] + STACK_SIZE - RET_POS;	// agarro el comienzo del stack
	
	*((uint64_t*) (stacks[pos] + STACK_SIZE - FLAGS_POS)) = FLAG_VALUES;						// tenemos que poner el flag de interrupcion en 1 y otros obligatorios

	*((uint64_t*) (stacks[pos] + STACK_SIZE - CS_POS)) = CS_VALUE;				
	*((uint64_t*) (stacks[pos] + STACK_SIZE - IP_POS)) = entrypoint;							// puntero al proceso que se va a correr

	tasks[pos].stackPointer = stacks[pos] + STACK_SIZE - STACK_POINT_OF_ENTRY;					// comienzo del stack
	tasks[pos].stackSegment = SS_VALUE;		

	tasks[pos].screen = screen;
	tasks[pos].pid = currentPid++;

	tasks[pos].isActive = ACTIVE_PROCESS;


	return tasks[pos].pid;
}


