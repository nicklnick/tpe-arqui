#include <juggler.h>
#include <stdint.h>					// ####### REMOVE #######


static uint8_t * vid = (uint8_t*)0xB8000;
static int offsetA;
static int offsetB = 80;
static int coso=0;


// ====================================================

int funcA(int param1, int screen){
	*(vid + offsetA) = 'A';
	*(vid + offsetA+2) = param1+'0';
	offsetA += 160;
	for(int k=0; k<50000000; k++);
	return param1-1;
}

int funcB(int param1, int screen){
	*(vid + offsetB) = 'B';
	*(vid + offsetB+2) = param1+'0';
	offsetB += 160;
	for(int k=0; k<50000000; k++);
	return param1-1;
}

// ====================================================


int main() {
	addCall(&funcA,10,1);
	addCall(&funcB,10,1);
	juggler();
}