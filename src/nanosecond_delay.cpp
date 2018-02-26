#include "nanosecond_delay.h"

void nanosecond_delay(unsigned int the_delay){
	unsigned int cycles = the_delay / (nanoseconds_per_second / F_CPU);
	for (unsigned int i=0; i<cycles; i++){
		asm volatile("nop");
		//continue;
	}
}