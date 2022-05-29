#include <idtLoader.h>
#include <lib.h>
#include <moduleLoader.h>
#include <stdint.h>
#include <string.h>

extern uint8_t text;
extern uint8_t rodata;
extern uint8_t data;
extern uint8_t bss;
extern uint8_t endOfKernelBinary;
extern uint8_t endOfKernel;

static const uint64_t PageSize = 0x1000;

static void * const sampleCodeModuleAddress = (void*)0x400000;
static void * const sampleDataModuleAddress = (void*)0x500000;

typedef int (*EntryPoint)();


void clearBSS(void * bssAddress, uint64_t bssSize) {
	memset(bssAddress, 0, bssSize);
}

void * getStackBase()
{
	return (void*)(
		(uint64_t)&endOfKernel
		+ PageSize * 8				//The size of the stack itself, 32KiB
		- sizeof(uint64_t)			//Begin at the top of the stack
	);
}

void * initializeKernelBinary()
{
	void * moduleAddresses[] = {
		sampleCodeModuleAddress,
		sampleDataModuleAddress
	};

	loadModules(&endOfKernelBinary, moduleAddresses);

	clearBSS(&bss, &endOfKernel - &bss);

	return getStackBase();
}


void punga1(){
	char i=0;
	while(1){
		i = (i+1) % 10 + '0';
		sys_write(3,&i,1);
		sys_write(3,"punga1\n",7);
	}
		
}

extern void div_by_zero();

void punga2(){
	div_by_zero();
}

int main()
{	
	sys_clear_screen();

	load_idt();
	
	addTask(&punga2, 5);
	addTask(&punga1, 3);
	enableMultiTasking();

	//((EntryPoint)sampleCodeModuleAddress)();	// llamada a userland
	return 0;
}
