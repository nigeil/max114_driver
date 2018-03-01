#include "nanosecond_delay.h"
#include <Arduino.h>

void nanosecond_delay(unsigned int the_delay){
	volatile unsigned int cycles_at_start = ARM_DWT_CYCCNT;
	unsigned int cycles_to_wait = the_delay / (nanoseconds_per_second / F_CPU);
	while((ARM_DWT_CYCCNT - cycles_at_start) < cycles_to_wait){
		continue;
	}
}