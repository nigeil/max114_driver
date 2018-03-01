#include <Arduino.h>

#include "Max114.h"
#include "global_defines.h"

// global constants
static const unsigned int n_repeats_to_avg = 100000;

// global objects
Max114* adc0;

// global variables
unsigned int iteration;
int analog_values[4];
long unsigned int start_time;
long unsigned int end_time;
double cumulative_time;

void setup(){
	Serial.begin(9600);
	delay(1000);
	Serial.println("hello world!");

	int data_pins[8];
	for(unsigned int i=0; i<8; i++){
		data_pins[i] = DATA_PINS[i];
		pinMode(DATA_PINS[i], INPUT);
	}
	int channel_pins[2];
	for(unsigned int i=0; i<2; i++){
		channel_pins[i] = CHANNEL_PINS[i];
		pinMode(CHANNEL_PINS[i], OUTPUT);
	}
	pinMode(CS_PIN, OUTPUT);
	pinMode(WR_PIN, OUTPUT);
	pinMode(RD_PIN, OUTPUT);
	pinMode(INT_PIN, INPUT);
	pinMode(MODE_PIN, OUTPUT);

	adc0 = new Max114(data_pins, channel_pins, CS_PIN, WR_PIN, RD_PIN, INT_PIN, MODE_PIN);
	adc0->set_mode(1);
	iteration = 0;
	for(unsigned int i=0; i<N_CHANNELS; i++){
		analog_values[i] = 0;
	}

	// start teensy cycle counter; normally in setup, here create it 
	// assuming it hasn't been made yet
	// Get cycle count as follows: current_cycles = ARM_DWT_CYCCNT;
	// TODO: make sure this is only called once across the entire
	// code's lifetime
	ARM_DEMCR |= ARM_DEMCR_TRCENA;  
  	ARM_DWT_CTRL |= ARM_DWT_CTRL_CYCCNTENA;
}

void loop(){
	start_time = micros();
	for(unsigned int i=0; i<4; i++) {
		analog_values[i] = adc0->analog_read(i);
	}
	end_time = micros();
	cumulative_time += (end_time - start_time);
	if (iteration == n_repeats_to_avg - 1){
		cumulative_time /= n_repeats_to_avg;
		Serial.print("Value = ");
		Serial.print(analog_values[0]);
		Serial.print(", ");
		Serial.print(analog_values[1]);
		Serial.print(", ");
		Serial.print(analog_values[2]);
		Serial.print(", ");
		Serial.print(analog_values[3]);
		Serial.print("\nAverage read time = ");
		Serial.print(cumulative_time);
		Serial.print("\n");
		cumulative_time = 0;
		iteration = 0;
	}
	else{
		iteration++;
	}
}