#ifndef MAX114_H
#define MAX114_H

#include <Arduino.h>

#include "global_defines.h"
#include "nanosecond_delay.h"

// Max114 device specific definitions
static const unsigned int N_BITS = 8;
static const unsigned int N_CHANNELS = 4;
static const unsigned int N_CHANNEL_PINS = 2;

class Max114 {

private:
	unsigned int data_pins[N_BITS];
	unsigned int channel_pins[N_CHANNEL_PINS];
	unsigned int RD_pin;
	unsigned int CS_pin;
	unsigned int WR_pin;
	unsigned int INT_pin;
	unsigned int MODE_pin;

	int analog_values[N_CHANNELS]; 
	int mode;

	int read_data_pins();
	int set_channel(int channel_number);

public:
	Max114(int* data_pins,int* channel_pins, int RD_pin, int CS_pin, int WR_pin, int INT_pin, int MODE_pin);
	int analog_read(unsigned int channel_number);
	int set_mode(unsigned int mode);
};

#endif