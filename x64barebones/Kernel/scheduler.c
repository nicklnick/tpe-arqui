// #include <stdint.h>


// typedef struct taskState{
// 	uint64_t rax;		// PopState popea estos
// 	uint64_t rbx;
// 	uint64_t rcx;
// 	uint64_t rdx;
// 	uint64_t rbp;
// 	uint64_t rdi;
// 	uint64_t rsi;
// 	uint64_t r8;
// 	uint64_t r9;
// 	uint64_t r10;
// 	uint64_t r11;
// 	uint64_t r12;
// 	uint64_t r13;
// 	uint64_t r14;
// 	uint64_t r15;

// 	uint64_t error; 	//????

// 	uint64_t rip;		// IRETQ popea estos
// 	uint64_t cs;
// 	uint64_t rflags;
// 	uint64_t esp;
// 	uint64_t ss;
// }taskState;

// //	esto es literal SO

// typedef struct stack{
// 	uint8_t stack[2048 - sizeof(taskState)];
// 	taskState state;
// }stack;

// #define STACKS_DIM 5

// static unsigned int currentTask;
// static unsigned int dim;
// static stack stacks[STACKS_DIM];

// void switchTask(){
// 	// ESP -> new task 
// 	// SS -> new task
// }

// void addTask(void (*function)(void) function){
// 	stacks[dim++]->state.rip = function;	 			//??????
// }