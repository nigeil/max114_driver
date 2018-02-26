#include "Max114.h"

Max114::Max114(int* data_pins_arg, int* channel_pins_arg, int RD_pin_arg, int CS_pin_arg, int WR_pin_arg, int INT_pin_arg, int MODE_pin_arg) {
	
	for (unsigned int i=0; i<N_BITS; i++){
		this->data_pins[i] = data_pins_arg[i];
	}

	for (unsigned int i=0; i<N_CHANNEL_PINS; i++){
		this->channel_pins[i] = channel_pins_arg[i];
	}

	this->RD_pin = RD_pin_arg;
	this->CS_pin = CS_pin_arg;
	this->WR_pin = WR_pin_arg;
	this->INT_pin = INT_pin_arg;
	this->MODE_pin = MODE_pin_arg;

	for (unsigned int i=0; i<N_CHANNELS; i++){
		this->analog_values[i] = -1; // -1 is default, "no known read value")
	}
}

int Max114::read_data_pins(){
	int value = 0;
	for (unsigned int i=0; i<N_BITS; i++){
		value |= digitalReadFast(this->data_pins[i]) << i;
	}
	return value;
}

int Max114::set_channel(int channel_number){
	unsigned int channel_pin_A0 = 0;
	unsigned int channel_pin_A1 = 0;

	switch (channel_number) {
		case 0: {
			channel_pin_A0 = 0;
			channel_pin_A1 = 0;
			break;
		}
		case 1: {
			channel_pin_A0 = 1;
			channel_pin_A1 = 0;
			break;
		}
		case 2: {
			channel_pin_A0 = 0;
			channel_pin_A1 = 1;
			break;
		}		
		case 3: {
			channel_pin_A0 = 1;
			channel_pin_A1 = 1;
			break;
		}
		default: {
			return -1;
		}
	}

	digitalWriteFast(this->channel_pins[0], channel_pin_A0);
	digitalWriteFast(this->channel_pins[1], channel_pin_A1);

	return 0;
}

int Max114::analog_read(unsigned int channel_number){
	unsigned int value = 0;

	if(this->mode == 0){
		// slower read only mode
		
		// select the channel
		set_channel(channel_number);

		// initiate the conversion by driving CS and RD low
		// hold RD low until data appears (INT goes low then high)
		digitalWriteFast(this->CS_pin, 0);
		digitalWriteFast(this->RD_pin, 0);

		while(digitalReadFast(this->INT_pin) == 1) {
			continue;
		}

		// release RD and read the value
		value = this->read_data_pins();
		digitalWriteFast(this->RD_pin, 1);
		digitalWriteFast(this->CS_pin, 1);

		return value;

	}
	else if (this->mode == 1){
		// fast read-write mode
		
		//select the channel
		set_channel(channel_number);
		
		// start the conversion
		digitalWriteFast(this->CS_pin, 0);
		digitalWriteFast(this->WR_pin, 0);
		nanosecond_delay(15);
		digitalWriteFast(this->WR_pin, 1);
		digitalWriteFast(this->CS_pin, 1);

		// wait for data to be ready
		while(digitalReadFast(this->INT_pin) == 1){
			Serial.println("INT is HIGH");
			continue;
		}

		// read the data when ready
		digitalWriteFast(this->CS_pin, 0);
		digitalWriteFast(this->RD_pin, 0);
		value = this->read_data_pins();
		digitalWriteFast(this->RD_pin, 1);
		digitalWriteFast(this->CS_pin, 1);

		// analog read done; return the value
		return value;
	}
	else{
		return -3; // error, not a valid mode
	}
}

int Max114::set_mode(unsigned int mode){
	switch (mode) {
		case 0: {
			digitalWriteFast(this->MODE_pin, 0);
			this->mode = 0;
			break;
		}
		case 1: {
			digitalWriteFast(this->MODE_pin, 1);
			this->mode = 1;
			break;
		}
		default: {
			digitalWriteFast(this->MODE_pin, 0);
			this->mode = 0;
			return -1; // error, not a valid mode (default to read mode)
		}
	}
	return 0;
}